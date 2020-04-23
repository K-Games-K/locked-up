#include "network/packet/player_move_packet.hpp"

PlayerMovePacket::PlayerMovePacket()
        : Packet(PACKET_ID)
{}

PlayerMovePacket::PlayerMovePacket(std::string nickname, int x, int y)
        : Packet(PACKET_ID), nickname(nickname), x(x), y(x)
{}

std::string PlayerMovePacket::get_nickname() const
{
    return nickname;
}

int PlayerMovePacket::get_x() const
{
    return x;
}

int PlayerMovePacket::get_y() const
{
    return y;
}

void PlayerMovePacket::serialize(sf::Packet& data) const
{
    data << nickname << x << y;
}

void PlayerMovePacket::deserialize(sf::Packet& data)
{
    data >> nickname >> x >> y;
}


