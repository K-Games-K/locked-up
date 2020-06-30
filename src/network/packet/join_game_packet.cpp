#include "join_game_packet.hpp"

JoinGamePacket::JoinGamePacket()
        : Packet(PACKET_ID)
{}

JoinGamePacket::JoinGamePacket(const std::string& nickname, const std::string& avatar_name)
        : Packet(PACKET_ID), nickname(nickname), avatar_name(avatar_name)
{}

std::string JoinGamePacket::get_nickname() const
{
    return nickname;
}

std::string JoinGamePacket::get_avatar_name() const
{
    return avatar_name;
}

void JoinGamePacket::serialize(sf::Packet& data) const
{
    data << nickname << avatar_name;
}

void JoinGamePacket::deserialize(sf::Packet& data)
{
    data >> nickname >> avatar_name;
}




