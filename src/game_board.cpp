#include "game_board.hpp"

int GameBoard::rooms_count() const
{
    return rooms.size();
}

Room& GameBoard::get_room(int room_id)
{
    return rooms.at(room_id);
}

const Room& GameBoard::get_room(int room_id) const
{
    return rooms.at(room_id);
}

Room& GameBoard::get_room(int x, int y)
{
    return rooms.at(tiles.at(x + y * width));
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

const std::vector<int>& GameBoard::get_tiles() const
{
    return tiles;
}

const std::vector<int>& GameBoard::get_neighbours(int room_id) const
{
    return neighbours_map.at(room_id);
}
