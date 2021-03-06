#include <ctime>

#include "set"
#include <numeric>
#include "game_board_loader.hpp"
#include "utils.hpp"
#include "logging.hpp"
#include "server/game_manager.hpp"

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
                    Packet::CountdownPacket countdown_packet;
                    countdown_packet.set_interval(COUNTDOWN_INTERVAL);
                    game_server.broadcast(countdown_packet);
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

                pmove_pos = gen_pmove_pos(
                        connected_players[current_player_id].get_position().x,
                        connected_players[current_player_id].get_position().y,
                        6,
                        pmove_pos
                );

                Packet::NewTurnPacket new_turn_packet;
                new_turn_packet.set_current_player_id(current_player_id);
                new_turn_packet.set_current_turn(turn);
                game_server.broadcast(new_turn_packet);
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

                game_server.broadcast(Packet::VotePacket());
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

                    std::string accused;
                    std::string winner;
                    std::vector<int> ac_ids;
                    for (int i = 0; i < results.size(); ++i)
                    {
                        if (results.at(i) == results.at(voting_result))
                        {
                            ac_ids.push_back(i);
                        }
                    }
                    accused = connected_players.at(ac_ids.at(0)).get_nickname();
                    for (int i = 1; i < ac_ids.size(); ++i)
                    {
                        accused += ", " + connected_players.at(ac_ids.at(i)).get_nickname();
                    }
                    
                    if (ac_ids.size() == 1 && ac_ids.at(0) == murderer_id) winner = "guests";
                    else winner = connected_players.at(murderer_id).get_nickname();

                    Packet::GameResultsPacket game_results_packet;
                    game_results_packet.set_murderer(connected_players.at(murderer_id).get_nickname());
                    game_results_packet.set_voting_result(accused);
                    *game_results_packet.mutable_voting_result_points() = { results.begin(), results.end() };
                    game_results_packet.set_winner(winner);
                    game_server.broadcast(game_results_packet);
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
    std::uniform_int_distribution<> random_alibi(0, ALIBI_LENGTH - 1);
    for(auto& player : connected_players)
    {
        player.generate_alibi(game_board, rand_room(gen), ALIBI_LENGTH);
        auto& alibi = player.get_alibi();
        uint32_t hide_room1 = 0;
        uint32_t hide_room2 = 0;
        uint32_t hide_room3 = 0;
        hide_room1 = random_alibi(gen);

        while(hide_room2 == hide_room3 || hide_room1 == hide_room2)
        {
            hide_room2 = random_alibi(gen);
            hide_room3 = random_alibi(gen);
        }

        for(int i = 0; i < alibi.size(); ++i)
        {
            if(i == hide_room1 || i == hide_room2 || i == hide_room3)
                continue;
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

        Packet::GameStartPacket game_start_packet;
        game_start_packet.set_crime_room(crime_room);
        game_start_packet.set_crime_item_name(crime_item.get_name());
        game_start_packet.set_crime_item_description(crime_item.get_description());
        game_start_packet.set_turns_per_game(TURNS_PER_GAME);
        for(auto& alibi : alibis)
        {
            *game_start_packet.add_alibis()->mutable_rooms() = {alibi.begin(), alibi.end()};
        }

        connected_players[i].get_connection().send(game_start_packet);
    }

    // Notify murderer.
    murderer.get_connection().send(Packet::MurdererPacket());

    // Randomize players starting positions.
    auto& tiles = game_board.get_tiles();
    for(auto& player : connected_players)
    {
        std::vector<int> room_tiles;
        int room = rand_room(gen);
        for(int i = 0; i < tiles.size(); ++i)
        {
            if(tiles[i] == room)
                room_tiles.push_back(i);
        }

        std::uniform_int_distribution<> rand_tile(0, room_tiles.size() - 1);
        int room_tile = room_tiles[rand_tile(gen)];
        int16_t x = room_tile % game_board.get_width();
        int16_t y = room_tile / game_board.get_width();
        player.set_position(x, y);

        Packet::PlayerMovePacket player_move_packet;
        player_move_packet.set_x(x);
        player_move_packet.set_y(y);
        player_move_packet.set_player_id(player.get_player_id());
        game_server.broadcast(player_move_packet);
    }
}

void GameManager::packet_received(RemotePlayer& sender, const Packet::Any& packet)
{
    auto& connection = sender.get_connection();
    auto& connected_players = game_server.get_connected_players();

    if(packet.Is<Packet::JoinGamePacket>() && game_stage == GameStage::Lobby)
    {
        Packet::JoinGamePacket join_game_packet;
        packet.UnpackTo(&join_game_packet);

        Packet::GameBoardPacket game_board_packet;
        GameBoardLoader::save_to_packet(game_board, game_board_packet);
        connection.send(game_board_packet);
    }
    else if(packet.Is<Packet::PlayerMovePacket>() && game_stage == GameStage::Movement)
    {
        if(current_player_id != sender.get_player_id())
            return;

        Packet::PlayerMovePacket player_move_packet;
        packet.UnpackTo(&player_move_packet);

        int x = player_move_packet.x();
        int y = player_move_packet.y();

        if(player_move_packet.relative())
        {
            int posx = sender.get_position().x;
            int posy = sender.get_position().y;
            int newx = posx + x;
            int newy = posy + y;

            if(newx < 0 || newx >= game_board.get_width() ||
               newy < 0 || newy >= game_board.get_height())
                return;

            if(std::find(pmove_pos.begin(), pmove_pos.end(), newx + newy * 100)
               == pmove_pos.end())
                return;

            if(game_board.can_move(posx, posy, x, y))
                sender.set_position(newx, newy);
            else
                return;

            game_server.broadcast(player_move_packet);
        }
        else if(teleport_allowed)
        {
            sender.set_position(x, y);
            game_server.broadcast(player_move_packet);
        }
    }
    else if(packet.Is<Packet::PlayerReadyPacket>() && game_stage == GameStage::Lobby)
    {
        Packet::PlayerReadyPacket player_ready_packet;
        packet.UnpackTo(&player_ready_packet);

        bool ready = player_ready_packet.ready();
        sender.set_ready(ready);

        game_server.broadcast(player_ready_packet);
    }
    else if(packet.Is<Packet::ClueFoundPacket>() &&
            (game_stage == GameStage::Action || game_stage == GameStage::Movement))
    {
        if(current_player_id != sender.get_player_id())
            return;

        Packet::ClueFoundPacket clue_found_packet;
        packet.UnpackTo(&clue_found_packet);

        game_stage = GameStage::Action;
        actions_left--;

        auto& player_room = game_board.get_room(
                sender.get_position().x, sender.get_position().y
        );
        auto& visitors = player_room.get_visitors();
        auto clues = visitors;

        std::uniform_real_distribution<> rand_perc(0, 100);
        if(!clues.empty() && rand_perc(gen) < 35)
        {
            std::shuffle(clues.begin(), clues.end(), gen);
            auto clue = clues.front();
            visitors.erase(std::find(visitors.begin(), visitors.end(), clue));

            clue_found_packet.Clear();
            clue_found_packet.set_clue(clue.second);
            clue_found_packet.set_time(hours.at(clue.first));
            connection.send(clue_found_packet);
        }
        else
        {
            clue_found_packet.Clear();
            connection.send(clue_found_packet);
        }
    }
    else if(packet.Is<Packet::FakeCluePacket>() &&
            (game_stage == GameStage::Action || game_stage == GameStage::Movement))
    {
        if(current_player_id != sender.get_player_id())
            return;

        Packet::FakeCluePacket fake_clue_packet;
        packet.UnpackTo(&fake_clue_packet);

        game_stage = GameStage::Action;
        actions_left--;

        auto& player_room = game_board.get_room(
                sender.get_position().x, sender.get_position().y
        );

        Log::debug() << fake_clue_packet.time() << ":"
                     << connected_players.at(fake_clue_packet.player_id()).get_nickname();

        player_room.get_visitors().emplace_back(
                fake_clue_packet.time(), connected_players[fake_clue_packet.player_id()].get_nickname()
        );
    }
    else if(packet.Is<Packet::VotePacket>() && game_stage == GameStage::Voting)
    {
        Packet::VotePacket vote_packet;
        packet.UnpackTo(&vote_packet);

        int vote_id = vote_packet.player_id();
        votes.at(sender.get_player_id()) = vote_id;
    }
    else
    {
        Log::warn() << "Received unhandled packet: " << packet.type_url() << "!" << std::endl;
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

std::set<int> GameManager::gen_pmove_pos(int x, int y, int move_count, std::set<int>& pmove_pos)
{
    if(move_count == 6)
    {
        pmove_pos.clear();
    }

    pmove_pos.insert(x + y * 100);

    if(move_count > 0)
    {
        if(game_board.can_move(x, y, -1, 0)) gen_pmove_pos(x - 1, y, move_count - 1, pmove_pos);
        if(game_board.can_move(x, y, +1, 0)) gen_pmove_pos(x + 1, y, move_count - 1, pmove_pos);
        if(game_board.can_move(x, y, 0, -1)) gen_pmove_pos(x, y - 1, move_count - 1, pmove_pos);
        if(game_board.can_move(x, y, 0, +1)) gen_pmove_pos(x, y + 1, move_count - 1, pmove_pos);
    }

    return pmove_pos;
}
