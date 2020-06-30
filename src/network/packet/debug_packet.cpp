#include "debug_packet.hpp"

DebugPacket::DebugPacket()
        : Packet(PACKET_ID)
{}

DebugPacket::DebugPacket(const std::string& debug_msg)
        : Packet(PACKET_ID), debug_msg(debug_msg)
{}

std::string DebugPacket::get_message() const
{
    return debug_msg;
}

void DebugPacket::serialize(sf::Packet& data) const
{
    data << debug_msg;
}

void DebugPacket::deserialize(sf::Packet& data)
{
    data >> debug_msg;
}

