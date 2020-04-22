#include "network/packet/debug_packet.hpp"
#include "network/serializer.hpp"
#include "network/deserializer.hpp"

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

std::vector<uint8_t> DebugPacket::serialize() const
{
    Serializer serializer(debug_msg.size() + 2);
    serializer.serialize(debug_msg);

    return serializer.finish();
}

bool DebugPacket::deserialize(std::vector<uint8_t> data)
{
    Deserializer deserializer(data);
    return deserializer.deserialize(debug_msg);
}
