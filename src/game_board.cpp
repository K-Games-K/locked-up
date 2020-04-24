#include "game_board.hpp"

GameBoard::GameBoard(const GameBoard& other)
        : width(other.width), height(other.height), rooms(other.rooms),
          collision_map(other.collision_map)
{
    tiles.reserve(other.tiles.size());
    for(auto& room_ref : other.tiles)
    {
        auto idx = std::distance(
                other.rooms.begin(),
                std::find(other.rooms.begin(), other.rooms.end(), room_ref.get())
        );

        tiles.push_back(rooms[idx]);
    }
}

GameBoard::GameBoard(int width, int height)
        : width(width), height(height)
{
    rooms.push_back(Room("<test_room_1>"));
    rooms.push_back(Room("<test_room_2>"));
    rooms.push_back(Room("<test_room_3>"));

    tiles.reserve(width * height);
    for(int i = 0; i < width * height; ++i)
    {
        if(i < 5 * width)
            tiles.push_back(rooms[0]);
        else if(i % width < width / 2)
            tiles.push_back(rooms[1]);
        else
            tiles.push_back(rooms[2]);
    }

    collision_map.reserve(width * height);
    for(int i = 0; i < width * height; ++i)
        collision_map.push_back({i % width != width / 2, true});
}

Room& GameBoard::get_room(int x, int y)
{
    return tiles.at(x + y * width).get();
}

bool GameBoard::can_move(int x, int y, int dirx, int diry)
{
    if (dirx == 1)
    {
        if(x == width - 1)
            return false;
        return !collision_map.at(x + y * width)[0];
    }
    else if (dirx == -1)
    {
        if(x == 0)
            return false;
        return !collision_map.at((x - 1) + y * width)[0];
    }
    else if (diry == 1)
    {
        if(y == height - 1)
            return false;
        return !collision_map.at(x + y * width)[1];
    }
    else if (diry == -1)
    {
        if(y == 0)
            return false;
        return !collision_map.at(x + (y - 1) * width)[1];
    }

    return false;
}


int GameBoard::get_width() const
{
    return width;
}

int GameBoard::get_height() const
{
    return height;
}

void GameBoard::operator=(const GameBoard& other)
{
    width = other.width;
    height = other.height;
    rooms = other.rooms;
    collision_map = other.collision_map;

    tiles.clear();
    tiles.reserve(other.tiles.size());
    for(auto& room_ref : other.tiles)
    {
        auto idx = std::distance(
                other.rooms.begin(),
                std::find(other.rooms.begin(), other.rooms.end(), room_ref.get())
        );

        tiles.push_back(rooms[idx]);
    }
}
