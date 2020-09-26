#pragma once

#include "packet.hpp"
#include "game_board.hpp"

class GameBoardPacket : public Packet
{
private:
    GameBoard game_board;

public:
    static constexpr uint16_t PACKET_ID = 0x5;

    GameBoardPacket();

    GameBoardPacket(GameBoard& game_board);

    GameBoard get_game_board() const;

    void serialize(sf::Packet& data) const override;

    void deserialize(sf::Packet& data) override;
};
