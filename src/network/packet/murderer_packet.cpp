#include "murderer_packet.hpp"

MurdererPacket::MurdererPacket()
    : Packet(PACKET_ID)
{}

void MurdererPacket::serialize(sf::Packet& data) const
{}

void MurdererPacket::deserialize(sf::Packet& data)
{}
