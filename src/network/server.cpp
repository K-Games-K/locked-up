#include <iostream>

#include "network/server.hpp"
#include "network/packet/packets.hpp"

Server::Server(unsigned short bind_port, sf::IpAddress bind_addr)
{
    listener.listen(bind_port, bind_addr);
    listener.setBlocking(false);
}

void Server::update()
{
    auto incoming = std::make_shared<sf::TcpSocket>();
    if(listener.accept(*incoming) == sf::Socket::Done)
    {
        new_connection(Connection(incoming));
    }

    std::vector<std::reference_wrapper<RemotePlayer>> to_remove;
    for(auto& player : players)
    {
        Connection& connection = player.get_connection();

        if(!connection.is_connected())
        {
            lost_connection(player);
            to_remove.push_back(player);
            continue;
        }

        auto packet = connection.recv();
        if(packet == nullptr)
            continue;

        packet_received(player, std::move(packet));
    }

    for(auto& player : to_remove)
        players.erase(std::find(players.begin(), players.end(), player));
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
                players_list.emplace_back(remote_player.get_nickname());
            broadcast(PlayersListPacket(players_list));

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

            if(player_move_packet.is_relative())
                player.move(player_move_packet.get_x(), player_move_packet.get_y());
            else
                player.set_position(player_move_packet.get_x(), player_move_packet.get_y());

            auto player_id = std::distance(
                    players.begin(),
                    std::find(players.begin(), players.end(), player)
            );

            player_move_packet.set_player_id(player_id);
            broadcast(player_move_packet);

            std::cout << "[" << player_id << "] move" << std::endl;

            break;
        }
        default:
            break;
    }
}

void Server::lost_connection(RemotePlayer& player)
{
    Connection& connection = player.get_connection();
    std::cout << "[" << connection.get_addr() << "] Lost connection!" << std::endl;
}

void Server::disconnected(RemotePlayer& player, const std::string& reason)
{
    Connection& connection = player.get_connection();

    std::cout << "[" << connection.get_addr() << "] " << player.get_nickname()
              << " left because: " << reason << std::endl;
}


