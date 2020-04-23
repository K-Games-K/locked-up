#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include "room.hpp"

class GameBoard
{
private:
    int width;
    int height;

    std::vector<Room> rooms;
    std::vector<std::reference_wrapper<Room>> tiles;

public:
    GameBoard(int width, int height);

    Room& get_room(int x, int y);
};
