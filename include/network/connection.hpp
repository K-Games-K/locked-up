#pragma once

#include <iostream>
#include <SFML/Network.hpp>

#include "packets.hpp"

class Connection
{
private:
    std::unique_ptr<sf::TcpSocket> socket;

    bool connected = false;

public:
    Connection() = default;

    Connection(sf::IpAddress remote_addr, unsigned short remote_port);

    explicit Connection(std::unique_ptr<sf::TcpSocket>&& socket);

    Connection(const Connection& other) = delete;

    Connection(Connection&& other) = default;

    sf::IpAddress get_addr() const;

    bool is_connected() const;

    void disconnect();

    bool send(const Packet::Packet& packet);

    std::optional<Packet::Any> recv();

    void operator=(const Connection& other) = delete;

    Connection& operator=(Connection&& other) = default;

    bool operator==(const Connection& other) const;
};
