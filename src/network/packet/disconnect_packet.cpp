#include "network/packet/disconnect_packet.hpp"

DisconnectPacket::DisconnectPacket(const std::string& reason)
        : Packet(PACKET_ID), reason(reason)
{}

std::string DisconnectPacket::get_reason() const
{
    return reason;
}

void DisconnectPacket::serialize(sf::Packet& data) const
{
    data << reason;
}

void DisconnectPacket::deserialize(sf::Packet& data)
{
    data >> reason;
}
