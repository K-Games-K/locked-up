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
    GameBoard() = default;

    GameBoard(const GameBoard& other);

    GameBoard(int width, int height);

    void load_from_indices(int width, int height, const std::vector<Room>& rooms,
                           const std::vector<int> tiles);

    void save_indices(std::vector<Room>& rooms, std::vector<int>& tiles) const;

    Room& get_room(int x, int y);

    int get_width() const;

    int get_height() const;
};
