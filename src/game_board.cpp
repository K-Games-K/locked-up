#include "game_board.hpp"

GameBoard::GameBoard(int width, int height)
        : width(width), height(height)
{
    rooms.push_back(Room("<test_room>"));
    tiles.reserve(width * height);
    for(int i = 0; i < width * height; ++i)
        tiles.push_back(rooms[0]);
}

Room& GameBoard::get_room(int x, int y)
{
    return tiles.at(x + y * width).get();
}
