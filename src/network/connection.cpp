#include "network/connection.hpp"

Connection::Connection(sf::IpAddress remote_addr, unsigned short remote_port)
        : socket(std::make_unique<sf::TcpSocket>())
{
    auto result = socket->connect(remote_addr, remote_port);
    if(result != sf::Socket::Done)
        return;

    socket->setBlocking(false);
    connected = true;
}

Connection::Connection(std::unique_ptr<sf::TcpSocket>&& socket)
        : socket(std::move(socket))
{
    this->socket->setBlocking(false);
    connected = true;
}

Connection::Connection(Connection&& other)
    : socket(std::move(other.socket)), connected(other.connected)
{
    other.socket = nullptr;
    other.connected = false;
}

sf::IpAddress Connection::get_addr() const
{
    return socket->getRemoteAddress();
}

bool Connection::is_connected() const
{
    return connected;
}

void Connection::disconnect()
{
    socket->disconnect();
    connected = false;
}

bool Connection::send(const Packet::Packet& packet)
{
    if(!connected)
        return false;

    Packet::Any any;
    any.PackFrom(packet);
    std::string data;
    if(!any.SerializeToString(&data))
        return false;

    sf::Packet message;
    message << data;
    auto result = socket->send(message);
    if(result == sf::Socket::Disconnected)
        connected = false;

    return result == sf::Socket::Done;
}

std::optional<Packet::Any> Connection::recv()
{
    if(!connected)
        return {};

    sf::Packet message;
    auto result = socket->receive(message);
    if(result == sf::Socket::Disconnected)
        connected = false;

    if(result != sf::Socket::Done)
        return {};

    std::string data;
    message >> data;
    Packet::Any packet;
    if(!packet.ParseFromString(data))
        return {};

    return packet;
}

bool Connection::operator==(const Connection& other) const
{
    return socket == other.socket;
}

