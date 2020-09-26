#include "network/packet/new_turn_packet.hpp"

NewTurnPacket::NewTurnPacket()
    : Packet(PACKET_ID)
{}

NewTurnPacket::NewTurnPacket(uint16_t current_player_id, uint16_t current_turn)
    : Packet(PACKET_ID), current_player_id(current_player_id), current_turn(current_turn)
{}

int NewTurnPacket::get_current_player_id() const
{
    return current_player_id;
}

int NewTurnPacket::get_current_turn() const
{
    return current_turn;
}

void NewTurnPacket::serialize(sf::Packet& data) const
{
    data << current_player_id << current_turn;
}

void NewTurnPacket::deserialize(sf::Packet& data)
{
    data >> current_player_id >> current_turn;
}


