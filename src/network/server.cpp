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

    for(auto& connection : connections)
    {
        if(!connection.is_connceted())
        {
            lost_connection(connection);
            continue;
        }

        auto packet = connection.recv();
        if(packet == nullptr)
            continue;

        packet_received(connection, std::move(packet));
    }
}

void Server::broadcast(const Packet& packet)
{
    for(auto& connection : connections)
    {
        connection.send(packet);
    }
}

void Server::new_connection(Connection connection)
{
    std::cout << "New connection from: " << connection.get_addr() << std::endl;

    connections.push_back(connection);
}

void Server::packet_received(Connection& sender, std::unique_ptr<Packet> packet)
{
    std::cout << "[" << sender.get_addr() << "] sent a packet!" << std::endl;

    switch(packet->get_id())
    {
        case DebugPacket::PACKET_ID:
        {
            auto debug_packet = dynamic_cast<DebugPacket&>(*packet);
            std::cout << "[Debug:" << sender.get_addr() << "]: "
                << debug_packet.get_message() << std::endl;

            break;
        }
        case JoinGamePacket::PACKET_ID:
        {
            auto join_game_packet = dynamic_cast<JoinGamePacket&>(*packet);

            std::string ip_addr = sender.get_addr().toString();
            std::string nickname = join_game_packet.get_nickname();

            Player player(nickname);
            connected_players.insert({ip_addr, player});

            break;
        }
        case DisconnectPacket::PACKET_ID:
        {
            auto disconnect_packet = dynamic_cast<DisconnectPacket&>(*packet);

            disconnected(sender, disconnect_packet.get_reason());

            break;
        }
        default:
            break;
    }
}

void Server::lost_connection(Connection& connection)
{
    std::cout << "[" << connection.get_addr() << "] Lost connection!" << std::endl;

    auto ip_addr = connection.get_addr().toString();
    connected_players.erase(ip_addr);
    connections.erase(std::find(connections.begin(), connections.end(), connection));
}

void Server::disconnected(Connection& connection, const std::string& reason)
{
    auto ip_addr = connection.get_addr().toString();
    auto player = connected_players.at(ip_addr);

    std::cout << "[" << connection.get_addr() << "] Left: " << player.get_nickname()
        << " because: " << reason << std::endl;

    connected_players.erase(ip_addr);
    connections.erase(std::find(connections.begin(), connections.end(), connection));
}


