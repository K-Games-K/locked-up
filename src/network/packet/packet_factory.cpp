#include "network/packet/packet_factory.hpp"
#include "network/packet/debug_packet.hpp"

std::unique_ptr<Packet> PacketFactory::create(uint16_t packet_id)
{
    switch(packet_id)
    {
        case DebugPacket::PACKET_ID:
            return std::make_unique<DebugPacket>();
        default:
            return nullptr;
    }
}
