#pragma once

#include <array>

#include "game_board.hpp"
#include "game_board_packet.pb.h"

class GameBoardLoader
{
public:
    static bool load_from_file(GameBoard& game_board, const std::string& filepath);

    static void load_from_packet(GameBoard& game_board, const Packet::GameBoardPacket& game_board_packet);
    static void save_to_packet(const GameBoard& game_board, Packet::GameBoardPacket& game_board_packet);
};
