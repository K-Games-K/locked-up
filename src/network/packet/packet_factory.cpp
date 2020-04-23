#include "network/packet/packet_factory.hpp"
#include "network/packet/packets.hpp"

std::unique_ptr<Packet> PacketFactory::create(uint16_t packet_id)
{
    switch(packet_id)
    {
        case DebugPacket::PACKET_ID:
            return std::make_unique<DebugPacket>();
        case JoinGamePacket::PACKET_ID:
            return std::make_unique<JoinGamePacket>();
        case PlayersListPacket::PACKET_ID:
            return std::make_unique<PlayersListPacket>();
        case DisconnectPacket::PACKET_ID:
            return std::make_unique<DisconnectPacket>();
        default:
            return nullptr;
    }
}
