#include "network/packet/vote_packet.hpp"

VotePacket::VotePacket()
    : Packet(PACKET_ID)
{}

VotePacket::VotePacket(uint16_t player_id)
    : Packet(PACKET_ID), player_id(player_id)
{}

uint16_t VotePacket::get_player_id() const
{
    return player_id;
}

void VotePacket::serialize(sf::Packet& data) const
{
    data << player_id;
}

void VotePacket::deserialize(sf::Packet& data)
{
    data >> player_id;
}
