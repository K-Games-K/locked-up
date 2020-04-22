#include "network/connection.hpp"
#include "network/packet/packet_factory.hpp"


Connection::Connection(std::shared_ptr<sf::TcpSocket>& socket)
        : socket(std::move(socket))
{
    this->socket->setBlocking(false);
}

Connection::Connection(sf::IpAddress remote_addr, unsigned short remote_port)
        : socket(std::make_shared<sf::TcpSocket>())
{
    socket->connect(remote_addr, remote_port);
    socket->setBlocking(false);
}

sf::IpAddress Connection::get_addr() const
{
    return socket->getRemoteAddress();
}

bool Connection::send(Packet& packet)
{
    sf::Packet data;
    data << packet.get_id();
    packet.serialize(data);

    return socket->send(data) == sf::Socket::Done;
}

std::unique_ptr<Packet> Connection::recv()
{
    sf::Packet data;
    if(socket->receive(data) != sf::Socket::Done)
        return nullptr;

    uint16_t packet_id = 0;
    data >> packet_id;

    auto packet = PacketFactory::create(packet_id);
    if(packet == nullptr)
        return nullptr;

    packet->deserialize(data);
    return packet;
}

