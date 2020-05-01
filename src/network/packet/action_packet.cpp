#include "network/packet/action_packet.hpp"

ActionPacket::ActionPacket()
    : Packet(PACKET_ID)
{}

ActionPacket::ActionPacket(ActionType action_type)
    : Packet(PACKET_ID), action_type(action_type)
{}

ActionType ActionPacket::get_action_type() const
{
    return action_type;
}

void ActionPacket::serialize(sf::Packet& data) const
{
    data << static_cast<uint8_t>(action_type);
}

void ActionPacket::deserialize(sf::Packet& data)
{
    uint8_t type;
    data >> type;
    action_type = static_cast<ActionType>(type);
}


