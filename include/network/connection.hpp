#pragma once

#include <SFML/Network.hpp>

#include "network/packet/packet.hpp"

class Connection
{
private:
    std::unique_ptr<sf::TcpSocket> socket;

public:
    explicit Connection(std::unique_ptr<sf::TcpSocket>& socket);

    bool send(Packet& packet);

    std::unique_ptr<Packet> recv();
};
