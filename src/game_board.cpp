#include "game_board.hpp"

GameBoard::GameBoard(const GameBoard& other)
        : width(other.width), height(other.height), rooms(other.rooms),
          collision_map(other.collision_map), neighbours_map(other.neighbours_map)
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

int GameBoard::rooms_count() const
{
    return rooms.size();
}

Room& GameBoard::get_room(int room_id)
{
    return rooms.at(room_id);
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

std::vector<int> GameBoard::get_neighbours(int room_id) const
{
    return neighbours_map.at(room_id);
}

void GameBoard::operator=(const GameBoard& other)
{
    width = other.width;
    height = other.height;
    rooms = other.rooms;
    collision_map = other.collision_map;
    neighbours_map = other.neighbours_map;

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
