#pragma once

#include <iostream>
#include <memory>
#include <SFML/Network.hpp>

#include "network/packet/packet.hpp"

class Connection
{
private:
    std::unique_ptr<sf::TcpSocket> socket;

    bool connected = false;

public:
    Connection() = default;

    Connection(std::unique_ptr<sf::TcpSocket>&& socket);

    Connection(sf::IpAddress remote_addr, unsigned short remote_port);

    Connection(Connection&& other);

    sf::IpAddress get_addr() const;

    bool is_connected() const;

    void disconnect();

    bool send(const Packet& packet);

    std::unique_ptr<Packet> recv();

    Connection& operator=(Connection&& other);

    bool operator==(const Connection& other) const;
};
