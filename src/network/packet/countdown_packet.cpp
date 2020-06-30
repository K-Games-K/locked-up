#include "countdown_packet.hpp"

CountdownPacket::CountdownPacket()
    : Packet(PACKET_ID)
{}

CountdownPacket::CountdownPacket(float interval)
    : Packet(PACKET_ID), interval(interval)
{}

float CountdownPacket::get_interval() const
{
    return interval;
}

void CountdownPacket::serialize(sf::Packet& data) const
{
    data << interval;
}

void CountdownPacket::deserialize(sf::Packet& data)
{
    data >> interval;
}


