#include "network/packet/game_start_packet.hpp"

GameStartPacket::GameStartPacket()
    : Packet(PACKET_ID)
{}

GameStartPacket::GameStartPacket(int16_t start_x, int16_t start_y)
    : Packet(PACKET_ID), start_x(start_x), start_y(start_y)
{}

int16_t GameStartPacket::get_start_x() const
{
    return start_x;
}

int16_t GameStartPacket::get_start_y() const
{
    return start_y;
}

void GameStartPacket::serialize(sf::Packet& data) const
{
    data << start_x << start_y;
}

void GameStartPacket::deserialize(sf::Packet& data)
{
    data >> start_x >> start_y;
}






