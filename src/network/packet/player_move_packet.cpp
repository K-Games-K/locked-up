#include "network/packet/player_move_packet.hpp"

PlayerMovePacket::PlayerMovePacket()
        : Packet(PACKET_ID)
{}

PlayerMovePacket::PlayerMovePacket(int16_t x, int16_t y, bool relative, uint16_t player_id)
        : Packet(PACKET_ID), x(x), y(y), player_id(player_id), relative(relative)
{}

void PlayerMovePacket::set_player_id(uint16_t player_id)
{
    this->player_id = player_id;
}

uint16_t PlayerMovePacket::get_player_id() const
{
    return player_id;
}

int16_t PlayerMovePacket::get_x() const
{
    return x;
}

int16_t PlayerMovePacket::get_y() const
{
    return y;
}

bool PlayerMovePacket::is_relative() const
{
    return relative;
}

void PlayerMovePacket::serialize(sf::Packet& data) const
{
    data << player_id << x << y << relative;
}

void PlayerMovePacket::deserialize(sf::Packet& data)
{
    data >> player_id >> x >> y >> relative;
}


