#pragma once

#include <iostream>
#include <memory>
#include <SFML/Network.hpp>

#include "network/packet/packet.hpp"

class Connection
{
private:
    std::shared_ptr<sf::TcpSocket> socket;

public:
    explicit Connection(std::shared_ptr<sf::TcpSocket>& socket);

    Connection(sf::IpAddress remote_addr, unsigned short remote_port);

    sf::IpAddress get_addr() const;

    bool send(Packet& packet);

    std::unique_ptr<Packet> recv();
};
