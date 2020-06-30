#include "clue_found_packet.hpp"

ClueFoundPacket::ClueFoundPacket()
    : Packet(PACKET_ID)
{}

ClueFoundPacket::ClueFoundPacket(std::string clue, std::string time)
    : Packet(PACKET_ID), clue(clue), time(time)
{}

std::string ClueFoundPacket::get_clue() const
{
    return clue;
}

std::string ClueFoundPacket::get_time() const
{
    return time;
}

void ClueFoundPacket::serialize(sf::Packet& data) const
{
    data << clue << time;
}

void ClueFoundPacket::deserialize(sf::Packet& data)
{
    data >> clue >> time;
}


