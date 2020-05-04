#include "game_board_loader.hpp"
#include "logging.hpp"
#include "server/game_manager.hpp"
#include "network/packet/packets.hpp"

GameManager::GameManager()
    : game_server(
    SERVER_ADDR, SERVER_PORT,
    std::bind(&GameManager::packet_received, this, std::placeholders::_1, std::placeholders::_2))
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

                if(timer.getElapsedTime().asSeconds() < COUNTDOWN_INTERVAL)
                    break;

                // Setting up new game.
                std::uniform_int_distribution<> rand_room(1, game_board.rooms_count() - 1);
                int crime_room = rand_room(gen);
                for(auto& player : connected_players)
                {
                    player.generate_alibi(game_board, crime_room, ALIBI_LENGTH);
                    auto& alibi = player.get_alibi();
                    for(int i = 0; i < alibi.size(); ++i)
                    {
                        auto& room = game_board.get_room(alibi[i]);
                        room.get_visitors().push_back({i, player.get_nickname()});
                    }
                }

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
                Item crime_item = items[rand_tool(gen)];

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
                            std::vector<int> indices(
                                alibi.size() - 1
                            ); // Last alibi is left untouched.
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

                murderer.get_connection().send(MurdererPacket());

                auto& tiles = game_board.get_tiles();
                std::vector<int> tiles_indices;
                for(int i = 0; i < tiles.size(); ++i)
                {
                    if(tiles[i] == crime_room)
                        tiles_indices.push_back(i);
                }
                std::shuffle(tiles_indices.begin(), tiles_indices.end(), gen);
                for(int i = 0; i < connected_players.size(); ++i)
                {
                    int16_t x = tiles_indices[i] % game_board.get_width();
                    int16_t y = tiles_indices[i] / game_board.get_width();
                    connected_players[i].set_position(x, y);
                    game_server.broadcast(
                        PlayerMovePacket(x, y, connected_players[i].get_player_id(), false));
                }

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

                game_server.broadcast(NewTurnPacket(current_player_id));
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

                votes.clear();
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
                break;
            }
        }
    }

    console_interfrace.shutdown();
    Log::info() << "Goodbye!" << std::endl;
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

                if(newx < 0 || newx >= game_board.get_width() ||
                    newy < 0 || newy >= game_board.get_height())
                    break;

                if(game_board.can_move(posx, posy, x, y))
                    sender.set_position(newx, newy);
                else
                    break;

                moves_left--;
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
        case ActionPacket::PACKET_ID:
        {
            if(game_stage != GameStage::Action && game_stage != GameStage::Movement ||
                current_player_id != sender.get_player_id())
                break;

            game_stage = GameStage::Action;

            auto action_packet = dynamic_cast<ActionPacket&>(*packet);
            ActionType action_type = action_packet.get_action_type();
            actions_left--;

            switch(action_type)
            {
                case ActionType::SearchRoom:
                {
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
                        connection.send(ClueFoundPacket(clue.second, hours.at(clue.first)));
                    }
                    else
                    {
                        connection.send(ClueFoundPacket());
                    }

                    break;
                }
                case ActionType::PlaceFalseClue:
                {
                    std::uniform_int_distribution<> rand_player(0, connected_players.size() - 1);
                    int player_id = sender.get_player_id();
                    while(player_id == sender.get_player_id())
                        player_id = rand_player(gen);

                    auto& player_room = game_board.get_room(
                        sender.get_position().x, sender.get_position().y
                    );

                    std::uniform_int_distribution<> rand_hour(0, hours.size() - 1);
                    player_room.get_visitors().push_back(
                        {rand_hour(gen), connected_players[player_id].get_nickname()}
                    );

                    break;
                }
            }

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

void GameManager::stop()
{
    enabled = false;
}
