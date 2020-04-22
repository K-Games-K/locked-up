#include "network/serializer.hpp"
#include "network/deserializer.hpp"
#include "network/packet/debug_packet.hpp"

DebugPacket::DebugPacket()
    : Packet(PACKET_ID)
{}

DebugPacket::DebugPacket(const std::string& debug_msg)
    : Packet(PACKET_ID), debug_msg(debug_msg)
{}

std::string DebugPacket::get_message() const
{
    return debug_msg;
}

void DebugPacket::serialize(Serializer& serializer) const
{
    serializer.serialize(debug_msg);
}

bool DebugPacket::deserialize(Deserializer& deserializer)
{
    return deserializer.deserialize(debug_msg);
}
