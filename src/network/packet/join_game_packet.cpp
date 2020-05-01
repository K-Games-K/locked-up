#include "network/packet/join_game_packet.hpp"

JoinGamePacket::JoinGamePacket()
        : Packet(PACKET_ID)
{}

JoinGamePacket::JoinGamePacket(const std::string& nickname, const std::string& avatar)
        : Packet(PACKET_ID), nickname(nickname), avatar(avatar)
{}

std::string JoinGamePacket::get_nickname() const
{
    return nickname;
}

std::string JoinGamePacket::get_avatar() const
{
    return avatar;
}

void JoinGamePacket::serialize(sf::Packet& data) const
{
    data << nickname << avatar;
}

void JoinGamePacket::deserialize(sf::Packet& data)
{
    data >> nickname >> avatar;
}




