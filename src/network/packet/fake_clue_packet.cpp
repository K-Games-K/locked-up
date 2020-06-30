#include "fake_clue_packet.hpp"

FakeCluePacket::FakeCluePacket()
    : Packet(PACKET_ID)
{
}

FakeCluePacket::FakeCluePacket(uint32_t time, uint32_t player_id)
    : Packet(PACKET_ID), time(time), player_id(player_id)
{}

void FakeCluePacket::serialize(sf::Packet& data) const
{
    data << time << player_id;
}

void FakeCluePacket::deserialize(sf::Packet& data)
{
    data >> time >> player_id;
}

uint32_t FakeCluePacket::get_time() const
{
    return time;
}

uint32_t FakeCluePacket::get_player_id() const
{
    return player_id;
}