#include <iostream>
#include <random>
#include <ctime>

#include "game_board_loader.hpp"
#include "network/server.hpp"
#include "network/packet/packets.hpp"

Server::Server(unsigned short bind_port, sf::IpAddress bind_addr)
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

            std::mt19937 gen(time(nullptr));
            std::uniform_int_distribution<> rand_room(0, game_board.rooms_count() - 1);
            int crime_room = rand_room(gen);
            std::cout << "Generating alibis. Crime room is "
                << game_board.get_room(crime_room).get_name() << "." << std::endl;
            for(auto& player : players)
            {
                player.generate_alibi(game_board, crime_room, ALIBI_LENGTH);
            }

            std::uniform_int_distribution<> rand_alibi();
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
                        std::random_shuffle(indices.begin(), indices.end());
                        alibis[j].resize(alibi.size(), -1);
                        for(int k = 0; k < VISIBLE_ALIBIS; ++k)
                            alibis[j][indices[k]] = alibi[indices[k]];
                    }
                }

                players[i].get_connection().send(GameStartPacket(i, i, alibis));
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
            actions_left = 0;
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
            game_stage = GameStage::NewTurn;
            break;
        }
        case GameStage::Voting:
        {
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
            player.set_nickname(nickname);

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


