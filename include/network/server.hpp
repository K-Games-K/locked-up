#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Network.hpp>

#include "player.hpp"
#include "connection.hpp"

class Server
{
private:
    sf::TcpListener listener;

    std::vector<Connection> connections;
    std::unordered_map<std::string, Player> connected_players;  // <IpAddress, Player>

    void new_connection(Connection connection);

    void packet_received(Connection& sender, std::unique_ptr<Packet> packet);

    void lost_connection(Connection& connection);

    void disconnected(Connection& connection, const std::string& reason);

public:
    Server(unsigned short bind_port, sf::IpAddress bind_addr = sf::IpAddress::Any);

    void update();

    void broadcast(const Packet& packet);
};
