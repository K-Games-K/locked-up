#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Network.hpp>

#include "remote_player.hpp"

class Server
{
private:
    sf::TcpListener listener;

    std::vector<RemotePlayer> players;

    void new_connection(Connection connection);

    void packet_received(RemotePlayer& sender, std::unique_ptr<Packet> packet);

    void lost_connection(RemotePlayer& player);

    void disconnected(RemotePlayer& player, const std::string& reason);

public:
    Server(unsigned short bind_port, sf::IpAddress bind_addr = sf::IpAddress::Any);

    void update();

    void broadcast(const Packet& packet);
};
