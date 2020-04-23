#include "network/packet/join_game_packet.hpp"

JoinGamePacket::JoinGamePacket()
        : Packet(PACKET_ID)
{}

JoinGamePacket::JoinGamePacket(const std::string& nickname)
        : Packet(PACKET_ID), nickname(nickname)
{}

std::string JoinGamePacket::get_nickname() const
{
    return nickname;
}

void JoinGamePacket::serialize(sf::Packet& data) const
{
    data << nickname;
}

void JoinGamePacket::deserialize(sf::Packet& data)
{
    data >> nickname;
}




