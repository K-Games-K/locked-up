#pragma once

#include <array>
#include "game_board.hpp"

class GameBoardLoader
{
public:
    static void save_in_memory(const GameBoard& game_board, int& width, int& height,
        std::vector<Room>& rooms, std::vector<int>& indices,
        std::vector<std::array<bool, 2>>& collision_map);

    static GameBoard load_from_memory(int width, int height, const std::vector<Room>& rooms,
        const std::vector<int>& indices, const std::vector<std::array<bool, 2>>& collision_map);

    static bool load_from_file(GameBoard& game_board, const std::string& filepath);
};
