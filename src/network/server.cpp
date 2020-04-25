#include <iostream>

#include "game_board_loader.hpp"
#include "network/server.hpp"
#include "network/packet/packets.hpp"

Server::Server(unsigned short bind_port, sf::IpAddress bind_addr)
{
   GameBoardLoader::load_from_file(game_board, "assets/maps/mapfile.karol");

    listener.listen(bind_port, bind_addr);
    listener.setBlocking(false);

    std::cout << "Server listening on " << bind_addr.toString() << ":"
              << listener.getLocalPort() << std::endl;
}

void Server::update()
{
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
        std::vector<Player> players_list;
        players_list.reserve(players.size());
        for(auto& remote_player : players)
            players_list.emplace_back(
                    remote_player.get_nickname(),
                    remote_player.get_position()
            );

        for(int i = 0; i < players.size(); ++i)
        {
            players[i].get_connection().send(PlayersListPacket(i, players_list));
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

    players.emplace_back(connection);
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
            auto join_game_packet = dynamic_cast<JoinGamePacket&>(*packet);

            std::string nickname = join_game_packet.get_nickname();
            player.set_nickname(nickname);

            std::vector<Player> players_list;
            players_list.reserve(players.size());
            for(auto& remote_player : players)
                players_list.emplace_back(
                        remote_player.get_nickname(),
                        remote_player.get_position()
                );


            for(int i = 0; i < players.size(); ++i)
            {
                players[i].get_connection().send(PlayersListPacket(i, players_list));
            }

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
            auto player_move_packet = dynamic_cast<PlayerMovePacket&>(*packet);

            int x = player_move_packet.get_x();
            int y = player_move_packet.get_y();

            if(player_move_packet.is_relative())
            {
                int posx = player.get_position().x;
                int posy = player.get_position().y;
                int newx = posx + x;
                int newy = posy + y;

                if(newx < 0 || newx >= GAMEBOARD_WIDTH || newy < 0 || newy >= GAMEBOARD_HEIGHT)
                    break;

                if(game_board.can_move(posx, posy, x, y))
                    player.set_position(newx, newy);
                else
                    break;
            }
            else
            {
                player.set_position(x, y);
            }

            // if(player_move_packet.is_relative())
            //     player.move(player_move_packet.get_x(), player_move_packet.get_y());
            // else
            //     player.set_position(player_move_packet.get_x(), player_move_packet.get_y());

            broadcast(player_move_packet);

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


