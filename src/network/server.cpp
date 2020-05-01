#include <numeric>
#include <algorithm>
#include <iostream>
#include <ctime>

#include "game_board_loader.hpp"
#include "network/server.hpp"
#include "network/packet/packets.hpp"

Server::Server(unsigned short bind_port, sf::IpAddress bind_addr)
    : gen((time(nullptr)))
{
    bool game_board_loaded = GameBoardLoader::load_from_file(
        game_board, "assets/maps/mapfile.karol"
    );

    if(!game_board_loaded)
    {
        std::cerr << "Failed to read mapfile!" << std::endl;
        return;
    }

    sf::Socket::Status status = listener.listen(bind_port, bind_addr);
    listener.setBlocking(false);

    std::string bind_str = bind_addr.toString() + ":" + std::to_string(bind_port);
    if(status != sf::Socket::Done)
    {
        std::cerr << "Failed to bind to " << bind_str << "!" << std::endl;
        return;
    }

    std::cout << "Server listening on " << bind_str << "..." << std::endl;
    enabled = true;
}

bool Server::is_enabled() const
{
    return enabled;
}

void Server::update()
{
    if(!enabled)
        return;

    auto incoming = std::make_shared<sf::TcpSocket>();
    if(listener.accept(*incoming) == sf::Socket::Done)
    {
        new_connection(Connection(incoming));
    }

    bool update_players_list = false;
    std::vector<std::reference_wrapper<RemotePlayer>> to_remove;
    for(auto& player : players)
    {
        Connection& connection = player.get_connection();

        if(!connection.is_connected())
        {
            disconnected(player, "Lost connection");
            to_remove.push_back(player);
            update_players_list = true;
            continue;
        }

        auto packet = connection.recv();
        if(packet == nullptr)
            continue;

        packet_received(player, std::move(packet));
    }

    for(auto& player : to_remove)
        players.erase(std::find(players.begin(), players.end(), player));

    if(update_players_list)
    {
        std::vector<Player> players_list(players.begin(), players.end());
        for(int i = 0; i < players.size(); ++i)
        {
            players[i].set_player_id(i);
            players[i].get_connection().send(PlayersListPacket(i, players_list));
        }
    }

    switch(game_stage)
    {
        case GameStage::Lobby:
        {
            bool ready = players.size() >= MIN_PLAYERS_COUNT;
            for(auto& player : players)
            {
                if(!player.is_ready())
                {
                    ready = false;
                    break;
                }
            }

            if(ready)
            {
                broadcast(CountdownPacket(COUNTDOWN_INTERVAL));
                game_stage = GameStage::Countdown;
                timer.restart();
            }

            break;
        }
        case GameStage::Countdown:
        {
            for(auto& player : players)
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
            std::cout << "Generating alibis. Crime room is "
                << game_board.get_room(crime_room).get_name() << "." << std::endl;
            for(auto& player : players)
            {
                player.generate_alibi(game_board, crime_room, ALIBI_LENGTH);
                auto& alibi = player.get_alibi();
                for(int i = 0; i < alibi.size(); ++i)
                {
                    auto& room = game_board.get_room(alibi[i]);
                    room.get_visitors().push_back({i, player.get_nickname()});
                }
            }

            std::uniform_int_distribution<> rand_player(0, players.size() - 1);
            auto& murderer = players[rand_player(gen)];
            murderer_id = murderer.get_player_id();
            std::vector<Item> items;
            for(int room_id : murderer.get_alibi())
            {
                auto& room = game_board.get_room(room_id);
                items.insert(items.end(), room.get_items().begin(), room.get_items().end());
            }
            std::uniform_int_distribution<> rand_tool(0, items.size() - 1);
            Item crime_item = items[rand_tool(gen)];

            for(int i = 0; i < players.size(); ++i)
            {
                std::vector<std::vector<int>> alibis(players.size());
                for(int j = 0; j < players.size(); ++j)
                {
                    if(i == j)
                    {
                        alibis[j] = players[i].get_alibi();
                    }
                    else
                    {
                        std::vector<int> alibi = players[j].get_alibi();
                        std::vector<int> indices(alibi.size() - 1); // Last alibi is left untouched.
                        std::iota(indices.begin(), indices.end(), 0);
                        std::shuffle(indices.begin(), indices.end(), gen);
                        alibis[j].resize(alibi.size(), -1);
                        for(int k = 0; k < VISIBLE_ALIBIS; ++k)
                            alibis[j][indices[k]] = alibi[indices[k]];
                    }
                }

                players[i].get_connection().send(
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
            for(int i = 0; i < players.size(); ++i)
            {
                int16_t x = tiles_indices[i] % game_board.get_width();
                int16_t y = tiles_indices[i] / game_board.get_width();
                players[i].set_position(x, y);
                broadcast(PlayerMovePacket(x, y, players[i].get_player_id(), false));
            }

            game_stage = GameStage::NewTurn;

            break;
        }
        case GameStage::NewTurn:
        {
            current_player_id++;
            if(current_player_id >= players.size())
            {
                turn++;
                current_player_id = 0;
            }

            broadcast(NewTurnPacket(current_player_id));
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
            votes.resize(players.size(), -1);

            broadcast(VotePacket());
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
                std::vector<int> results(players.size(), 0);
                for(int vote : votes)
                    results[vote]++;

                int voting_result = *std::max_element(results.begin(), results.end());
                broadcast(
                    GameResultsPacket(
                        players.at(murderer_id).get_nickname(),
                        players.at(voting_result).get_nickname()
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

void Server::broadcast(const Packet& packet)
{
    for(auto& player : players)
    {
        player.get_connection().send(packet);
    }
}

void Server::new_connection(Connection connection)
{
    std::cout << "New connection from: " << connection.get_addr() << std::endl;

    players.emplace_back(players.size(), connection);
}

void Server::packet_received(RemotePlayer& player, std::unique_ptr<Packet> packet)
{
    Connection& connection = player.get_connection();
    std::cout << "[" << connection.get_addr() << "] sent a packet of id: "
        << packet->get_id() << std::endl;

    switch(packet->get_id())
    {
        case DebugPacket::PACKET_ID:
        {
            auto debug_packet = dynamic_cast<DebugPacket&>(*packet);
            std::cout << "[Debug:" << connection.get_addr() << "]: "
                << debug_packet.get_message() << std::endl;

            break;
        }
        case JoinGamePacket::PACKET_ID:
        {
            if(game_stage != GameStage::Lobby)
                break;

            auto join_game_packet = dynamic_cast<JoinGamePacket&>(*packet);

            std::string nickname = join_game_packet.get_nickname();
            std::string avatar = join_game_packet.get_avatar();
            player.set_nickname(nickname);
            player.set_avatar(avatar);
            std::cout << avatar << std::endl;

            std::vector<Player> players_list(players.begin(), players.end());
            for(int i = 0; i < players.size(); ++i)
                players[i].get_connection().send(PlayersListPacket(i, players_list));

            GameBoardPacket game_board_packet(game_board);
            connection.send(game_board_packet);

            break;
        }
        case DisconnectPacket::PACKET_ID:
        {
            auto disconnect_packet = dynamic_cast<DisconnectPacket&>(*packet);

            disconnected(player, disconnect_packet.get_reason());

            break;
        }
        case PlayerMovePacket::PACKET_ID:
        {
            if(game_stage != GameStage::Movement || current_player_id != player.get_player_id())
                break;

            auto player_move_packet = dynamic_cast<PlayerMovePacket&>(*packet);

            int x = player_move_packet.get_x();
            int y = player_move_packet.get_y();

            if(player_move_packet.is_relative())
            {
                int posx = player.get_position().x;
                int posy = player.get_position().y;
                int newx = posx + x;
                int newy = posy + y;

                if(newx < 0 || newx >= game_board.get_width() ||
                    newy < 0 || newy >= game_board.get_height())
                    break;

                if(game_board.can_move(posx, posy, x, y))
                    player.set_position(newx, newy);
                else
                    break;

                moves_left--;
                broadcast(player_move_packet);
            }
            else if(teleport_allowed)
            {
                player.set_position(x, y);
                broadcast(player_move_packet);
            }

            break;
        }
        case PlayerReadyPacket::PACKET_ID:
        {
            if(game_stage != GameStage::Lobby)
                break;

            auto player_ready_packet = dynamic_cast<PlayerReadyPacket&>(*packet);
            bool ready = player_ready_packet.is_ready();
            player.set_ready(ready);

            broadcast(player_ready_packet);

            break;
        }
        case ActionPacket::PACKET_ID:
        {
            if(game_stage != GameStage::Action && game_stage != GameStage::Movement ||
                current_player_id != player.get_player_id())
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
                        player.get_position().x, player.get_position().y
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
                    std::uniform_int_distribution<> rand_player(0, players.size() - 1);
                    int player_id = player.get_player_id();
                    while(player_id == player.get_player_id())
                        player_id = rand_player(gen);

                    auto& player_room = game_board.get_room(
                        player.get_position().x, player.get_position().y
                    );

                    std::uniform_int_distribution<> rand_hour(0, hours.size() - 1);
                    player_room.get_visitors().push_back(
                        {rand_hour(gen), players[player_id].get_nickname()}
                    );

                    for(auto& a : player_room.get_visitors())
                        std::cout << a.first << ":" << a.second << std::endl;

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
            votes.at(player.get_player_id()) = vote_id;

            break;
        }
        default:
            break;
    }
}

void Server::disconnected(RemotePlayer& player, const std::string& reason)
{
    Connection& connection = player.get_connection();

    std::cout << "[" << connection.get_addr() << "] " << player.get_nickname()
        << " left because: " << reason << std::endl;
}
