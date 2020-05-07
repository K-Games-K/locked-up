#include <ctime>

#include "set"
#include <numeric>
#include "game_board_loader.hpp"
#include "utils.hpp"
#include "logging.hpp"
#include "server/game_manager.hpp"
#include "network/packet/packets.hpp"

GameManager::GameManager()
    : game_server(
    SERVER_ADDR, SERVER_PORT,
    std::bind(&GameManager::packet_received, this, std::placeholders::_1, std::placeholders::_2)),
    gen(time(nullptr))
{
    if(!game_server.is_enabled())
    {
        Log::error() << "Failed to initialize game server!" << std::endl;
        return;
    }

    Log::info() << "Initializing game manager..." << std::endl;

    const std::string mapfile = "assets/maps/mapfile.karol";
    if(!GameBoardLoader::load_from_file(game_board, mapfile))
    {
        Log::error() << "Failed to read mapfile: " << mapfile << "!" << std::endl;
        return;
    }
    Log::info() << "Loaded mapfile: " << mapfile << std::endl;

    Log::info() << "Registering console interface commands..." << std::endl;
    console_interfrace.register_command(
        "set", std::bind(&GameManager::set, this, std::placeholders::_1)
    );
    console_interfrace.register_command("stop", std::bind(&GameManager::stop, this));
    Log::info() << "Console interface ready!" << std::endl;

    enabled = true;
    Log::info() << "Game manager initialized successfully!" << std::endl;
}

void GameManager::run()
{
    if(!enabled)
        Log::error() << "Failed to initialize game manager!" << std::endl;

    while(enabled)
    {
        game_server.update();

        auto& connected_players = game_server.get_connected_players();

        switch(game_stage)
        {
            case GameStage::Lobby:
            {
                // Wait for all players to get ready.
                bool ready = connected_players.size() >= MIN_PLAYERS_COUNT;
                for(auto& player : connected_players)
                {
                    if(!player.is_ready())
                    {
                        ready = false;
                        break;
                    }
                }

                if(ready)
                {
                    game_server.broadcast(CountdownPacket(COUNTDOWN_INTERVAL));
                    game_stage = GameStage::Countdown;
                    timer.restart();
                }

                break;
            }
            case GameStage::Countdown:
            {
                for(auto& player : connected_players)
                {
                    if(!player.is_ready())
                    {
                        game_stage = GameStage::Lobby;
                        break;
                    }
                }

                // Wait for countdown to finish.
                if(timer.getElapsedTime().asSeconds() < COUNTDOWN_INTERVAL)
                    break;

                game_server.set_allow_new_connections(false);

                prepare_new_game();

                game_stage = GameStage::NewTurn;

                break;
            }
            case GameStage::NewTurn:
            {
                current_player_id++;
                if(current_player_id >= connected_players.size())
                {
                    turn++;
                    current_player_id = 0;
                }
                for (auto& player : connected_players)
                {
                    player.set_pmove_pos(gen_pmove_pos(
                        player.get_position().x,
                        player.get_position().y,
                        6,
                        player.get_pmove_pos()
                    ));
                }
                
                game_server.broadcast(NewTurnPacket(current_player_id, turn));
                moves_left = MOVES_PER_TURN;
                actions_left = ACTIONS_PER_TURN;
                game_stage = GameStage::Movement;
                break;
            }
            case GameStage::Movement:
            {
                if(moves_left <= 0)
                    game_stage = GameStage::Action;
                break;
            }
            case GameStage::Action:
            {
                if(actions_left <= 0)
                    game_stage = GameStage::EndTurn;
                break;
            }
            case GameStage::EndTurn:
            {
                if(turn < TURNS_PER_GAME)
                {
                    game_stage = GameStage::NewTurn;
                    break;
                }

                // Game ends.
                votes.resize(connected_players.size(), -1);

                game_server.broadcast(VotePacket());
                game_stage = GameStage::Voting;
                break;
            }
            case GameStage::Voting:
            {
                bool voting_done = true;
                for(int vote : votes)
                {
                    if(vote == -1)
                    {
                        voting_done = false;
                        break;
                    }
                }

                // Votig finished.
                if(voting_done)
                {
                    std::vector<int> results(connected_players.size(), 0);
                    for(int vote : votes)
                        results.at(vote)++;

                    int voting_result = std::distance(
                        results.begin(),
                        std::max_element(results.begin(), results.end())
                    );
                    game_server.broadcast(
                        GameResultsPacket(
                            connected_players.at(murderer_id).get_nickname(),
                            connected_players.at(voting_result).get_nickname()
                        )
                    );

                    game_stage = GameStage::Results;
                }

                break;
            }
            case GameStage::Results:
            {
                game_server.reset();
                game_stage = GameStage::Lobby;
                break;
            }
        }
    }

    console_interfrace.shutdown();
    Log::info() << "Goodbye!" << std::endl;
}

void GameManager::prepare_new_game()
{
    auto& connected_players = game_server.get_connected_players();

    // Reset server.
    current_player_id = 0;
    turn = 0;
    votes.clear();

    // Generate alibis.
    std::uniform_int_distribution<> rand_room(1, game_board.rooms_count() - 1);
    for(auto& player : connected_players)
    {
        player.generate_alibi(game_board, rand_room(gen), ALIBI_LENGTH);
        auto& alibi = player.get_alibi();
        for(int i = 0; i < alibi.size(); ++i)
        {
            auto& room = game_board.get_room(alibi[i]);
            room.get_visitors().push_back({i, player.get_nickname()});
        }
    }

    // Select murderer and crime tool.
    std::uniform_int_distribution<> rand_player(0, connected_players.size() - 1);
    auto& murderer = connected_players[rand_player(gen)];
    murderer_id = murderer.get_player_id();
    std::vector<Item> items;
    for(int room_id : murderer.get_alibi())
    {
        auto& room = game_board.get_room(room_id);
        items.insert(items.end(), room.get_items().begin(), room.get_items().end());
    }
    std::uniform_int_distribution<> rand_tool(0, items.size() - 1);
    auto& crime_item = items[rand_tool(gen)];
    int crime_room = murderer.get_alibi()[ALIBI_LENGTH - 1];

    // Prepare incomplete alibis and send them to players.
    for(int i = 0; i < connected_players.size(); ++i)
    {
        std::vector<std::vector<int>> alibis(connected_players.size());
        for(int j = 0; j < connected_players.size(); ++j)
        {
            if(i == j)
            {
                alibis[j] = connected_players[i].get_alibi();
            }
            else
            {
                std::vector<int> alibi = connected_players[j].get_alibi();
                std::vector<int> indices(alibi.size() - 1);
                std::iota(indices.begin(), indices.end(), 0);
                std::shuffle(indices.begin(), indices.end(), gen);
                alibis[j].resize(alibi.size(), -1);
                for(int k = 0; k < VISIBLE_ALIBIS; ++k)
                    alibis[j][indices[k]] = alibi[indices[k]];
            }
        }

        connected_players[i].get_connection().send(
            GameStartPacket(i, i, alibis, crime_room, crime_item)
        );
    }

    // Notify murderer.
    murderer.get_connection().send(MurdererPacket());

    // Randomize players starting positions.
    auto& tiles = game_board.get_tiles();
    for(auto& player : connected_players)
    {
        std::vector<int> room_tiles;
        for(int i = 0; i < tiles.size(); ++i)
        {
            if(tiles[i] == player.get_alibi()[ALIBI_LENGTH - 1])
                room_tiles.push_back(i);
        }

        std::uniform_int_distribution<> rand_tile(0, room_tiles.size() - 1);
        int16_t x = room_tiles[rand_tile(gen)] % game_board.get_width();
        int16_t y = room_tiles[rand_tile(gen)] / game_board.get_width();
        player.set_position(x, y);

        game_server.broadcast(PlayerMovePacket(x, y, player.get_player_id(), false));
    }
}

void GameManager::packet_received(RemotePlayer& sender, std::unique_ptr<Packet> packet)
{
    auto& connection = sender.get_connection();
    auto& connected_players = game_server.get_connected_players();

    switch(packet->get_id())
    {
        case JoinGamePacket::PACKET_ID:
        {
            if(game_stage != GameStage::Lobby)
                break;

            connection.send(GameBoardPacket(game_board));
        }
        case PlayerMovePacket::PACKET_ID:
        {
            if(game_stage != GameStage::Movement || current_player_id != sender.get_player_id())
                break;

            auto player_move_packet = dynamic_cast<PlayerMovePacket&>(*packet);

            int x = player_move_packet.get_x();
            int y = player_move_packet.get_y();

            if(player_move_packet.is_relative())
            {
                int posx = sender.get_position().x;
                int posy = sender.get_position().y;
                int newx = posx + x;
                int newy = posy + y;
                std::set<int> pmove_pos = sender.get_pmove_pos();

                if(newx < 0 || newx >= game_board.get_width() ||
                    newy < 0 || newy >= game_board.get_height())
                    break;

                Log::debug() << "*" << std::endl;

                if (std::find(pmove_pos.begin(), pmove_pos.end(), newx + newy * 100)
                    == pmove_pos.end())
                    break;

                Log::debug() << "*" << std::endl;

                if(game_board.can_move(posx, posy, x, y))
                    sender.set_position(newx, newy);
                else
                    break;

                game_server.broadcast(player_move_packet);
            }
            else if(teleport_allowed)
            {
                sender.set_position(x, y);
                game_server.broadcast(player_move_packet);
            }

            break;
        }
        case PlayerReadyPacket::PACKET_ID:
        {
            if(game_stage != GameStage::Lobby)
                break;

            auto player_ready_packet = dynamic_cast<PlayerReadyPacket&>(*packet);
            bool ready = player_ready_packet.is_ready();
            sender.set_ready(ready);

            game_server.broadcast(player_ready_packet);

            break;
        }
        case ClueFoundPacket::PACKET_ID:
        {
            if (game_stage != GameStage::Action && game_stage != GameStage::Movement ||
                current_player_id != sender.get_player_id())
                break;

            game_stage = GameStage::Action;
            actions_left--;

            auto& player_room = game_board.get_room(
                sender.get_position().x, sender.get_position().y
            );
            auto& visitors = player_room.get_visitors();
            auto clues = visitors;

            std::uniform_real_distribution<> rand_perc(0, 100);
            if (!clues.empty() && rand_perc(gen) < 35)
            {
                std::shuffle(clues.begin(), clues.end(), gen);
                auto clue = clues.front();
                visitors.erase(std::find(visitors.begin(), visitors.end(), clue));
                connection.send(ClueFoundPacket(clue.second, hours.at(clue.first)));
            }
            else
            {
                connection.send(ClueFoundPacket());
            }

            break;
        }
        case FakeCluePacket::PACKET_ID:
        {
            if(game_stage != GameStage::Action && game_stage != GameStage::Movement ||
                current_player_id != sender.get_player_id())
                break;

            game_stage = GameStage::Action;

            actions_left--;

            auto fake_clue_packet = dynamic_cast<FakeCluePacket&>(*packet);

            auto& player_room = game_board.get_room(
                sender.get_position().x, sender.get_position().y
            );

            Log::debug() << fake_clue_packet.get_time() << ":" << connected_players[fake_clue_packet.get_player_id()].get_nickname();

            player_room.get_visitors().push_back(
                { fake_clue_packet.get_time(), connected_players[fake_clue_packet.get_player_id()].get_nickname()}
            );


            break;
        }
        case VotePacket::PACKET_ID:
        {
            if(game_stage != GameStage::Voting)
                break;

            auto vote_packet = dynamic_cast<VotePacket&>(*packet);
            int vote_id = vote_packet.get_player_id();
            votes.at(sender.get_player_id()) = vote_id;

            break;
        }
        default:
            break;
    }
}

void GameManager::set(const std::vector<std::string>& args)
{
    try
    {
        if(args[1] == "countdown_interval")
        {
            this->COUNTDOWN_INTERVAL = std::stof(args[2]);
        }
        else if(args[1] == "min_players")
        {
            this->MIN_PLAYERS_COUNT = std::stoi(args[2]);
        }
        else if(args[1] == "moves")
        {
            this->MOVES_PER_TURN = std::stoi(args[2]);
        }
        else if(args[1] == "actions")
        {
            this->ACTIONS_PER_TURN = std::stoi(args[2]);
        }
        else if(args[1] == "turns")
        {
            this->TURNS_PER_GAME = std::stoi(args[2]);
        }
        else if(args[1] == "visible_alibis")
        {
            this->VISIBLE_ALIBIS = std::stoi(args[2]);
        }
        else
        {
            Log::warn()
                << "Usage: set countdown_interval/min_players/moves/actions/turns/visible_alibis <value>"
                << std::endl;
        }
    }
    catch(std::invalid_argument e)
    {
        Log::warn() << "Invalid value." << std::endl;
    }
    catch(std::out_of_range e)
    {
        Log::warn() << "Value out of range." << std::endl;
    }
}

void GameManager::stop()
{
    enabled = false;
}

std::set<int> GameManager::gen_pmove_pos(int x, int y, int move_count, std::set<int> &pmove_pos)
{
    if (move_count == 6)
    {
        std::cout << "pmove_cleared" << std::endl;
        pmove_pos.clear();
    }

    pmove_pos.insert(x + y * 100);

    if (move_count > 0)
    {
        if(game_board.can_move(x, y, - 1, 0))     gen_pmove_pos(x - 1, y, move_count - 1, pmove_pos);
        if (game_board.can_move(x, y, + 1, 0))    gen_pmove_pos(x + 1, y, move_count - 1, pmove_pos);
        if (game_board.can_move(x, y, 0, - 1))    gen_pmove_pos(x, y - 1, move_count - 1, pmove_pos);
        if (game_board.can_move(x, y, 0, + 1))    gen_pmove_pos(x, y + 1, move_count - 1, pmove_pos);       
    }

    return pmove_pos;
}