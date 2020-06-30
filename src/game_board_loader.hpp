#pragma once

#include <array>
#include "game_board.hpp"

class GameBoardLoader
{
public:
    static bool load_from_file(GameBoard& game_board, const std::string& filepath);
};
