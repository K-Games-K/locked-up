#include "network/packet/player_move_packet.hpp"

PlayerMovePacket::PlayerMovePacket()
        : Packet(PACKET_ID)
{}

PlayerMovePacket::PlayerMovePacket(uint16_t player_id, uint16_t x, uint16_t y)
        : Packet(PACKET_ID), x(x), y(x)
{}

void PlayerMovePacket::set_player_id(uint16_t player_id)
{
    this->player_id = player_id;
}

uint16_t PlayerMovePacket::get_player_id() const
{
    return player_id;
}

uint16_t PlayerMovePacket::get_x() const
{
    return x;
}

uint16_t PlayerMovePacket::get_y() const
{
    return y;
}

void PlayerMovePacket::serialize(sf::Packet& data) const
{
    data << player_id << x << y;
}

void PlayerMovePacket::deserialize(sf::Packet& data)
{
    data >> player_id >> x >> y;
}


