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
        case PlayerMovePacket::PACKET_ID:
            return std::make_unique<PlayerMovePacket>();
        case GameBoardPacket::PACKET_ID:
            return std::make_unique<GameBoardPacket>();
        case PlayerReadyPacket::PACKET_ID:
            return std::make_unique<PlayerReadyPacket>();
        case CountdownPacket::PACKET_ID:
            return std::make_unique<CountdownPacket>();
        case GameStartPacket::PACKET_ID:
            return std::make_unique<GameStartPacket>();
        case NewTurnPacket::PACKET_ID:
            return std::make_unique<NewTurnPacket>();
        case FakeCluePacket::PACKET_ID:
            return std::make_unique<FakeCluePacket>();
        case ClueFoundPacket::PACKET_ID:
            return std::make_unique<ClueFoundPacket>();
        case MurdererPacket::PACKET_ID:
            return std::make_unique<MurdererPacket>();
        case VotePacket::PACKET_ID:
            return std::make_unique<VotePacket>();
        case GameResultsPacket::PACKET_ID:
            return std::make_unique<GameResultsPacket>();
        default:
            return nullptr;
    }
}
