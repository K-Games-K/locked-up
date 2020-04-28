#include "network/packet/player_ready_packet.hpp"

PlayerReadyPacket::PlayerReadyPacket()
    : Packet(PACKET_ID)
{}

PlayerReadyPacket::PlayerReadyPacket(int player_id, bool ready)
    : Packet(PACKET_ID), player_id(player_id), ready(ready)
{}

int PlayerReadyPacket::get_player_id() const
{
    return player_id;
}

bool PlayerReadyPacket::is_ready() const
{
    return ready;
}

void PlayerReadyPacket::serialize(sf::Packet& data) const
{
    data << player_id << ready;
}

void PlayerReadyPacket::deserialize(sf::Packet& data)
{
    data >> player_id >> ready;
}
