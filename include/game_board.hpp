#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include "room.hpp"

class GameBoard
{
private:
    friend class GameBoardLoader;

    int width;
    int height;

    std::vector<Room> rooms;
    std::vector<std::reference_wrapper<Room>> tiles;
    std::vector<std::array<bool, 2>> collision_map;

public:
    GameBoard() = default;

    GameBoard(const GameBoard& other);

    GameBoard(int width, int height);

    Room& get_room(int x, int y);

    bool can_move(int x, int y, int dirx, int diry);

    int get_width() const;

    int get_height() const;

    void operator=(const GameBoard& other);
};
