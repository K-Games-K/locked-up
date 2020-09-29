#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <functional>

#include "room.hpp"

class GameBoard
{
private:
    friend class GameBoardLoader;

    int width = 0;
    int height = 0;

    std::vector<Room> rooms;
    std::vector<int> tiles;
    std::vector<std::array<bool, 2>> collision_map;
    std::vector<std::vector<int>> neighbours_map;

public:
    GameBoard() = default;

    int rooms_count() const;

    Room& get_room(int room_id);

    const Room& get_room(int room_id) const;

    Room& get_room(int x, int y);

    bool can_move(int x, int y, int dirx, int diry);

    int get_width() const;

    int get_height() const;

    const std::vector<int>& get_tiles() const;

    const std::vector<int>& get_neighbours(int room_id) const;
};
