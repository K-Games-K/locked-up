#include <fstream>
#include <string>
#include <sstream>

#include "game_board_loader.hpp"

void GameBoardLoader::save_in_memory(const GameBoard& game_board, int& width, int& height,
                                     std::vector<Room>& rooms, std::vector<int>& indices,
                                     std::vector<std::array<bool, 2>>& collision_map)
{
    width = game_board.width;
    height = game_board.height;
    rooms = game_board.rooms;
    collision_map = game_board.collision_map;

    indices.reserve(width * height);
    for(auto& room_ref : game_board.tiles)
    {
        auto idx = std::distance(
                game_board.rooms.begin(),
                std::find(game_board.rooms.begin(), game_board.rooms.end(), room_ref.get())
        );

        indices.push_back(idx);
    }
}

GameBoard GameBoardLoader::load_from_memory(int width, int height, const std::vector<Room>& rooms,
                                            const std::vector<int>& indices,
                                            const std::vector<std::array<bool, 2>>& collision_map)
{
    GameBoard game_board;
    game_board.width = width;
    game_board.height = height;
    game_board.rooms = rooms;
    game_board.collision_map = collision_map;

    game_board.tiles.reserve(width * height);
    for(auto i : indices)
        game_board.tiles.push_back(game_board.rooms[i]);

    return game_board;
}

bool GameBoardLoader::load_from_file(GameBoard& game_board, const std::string& filepath)
{
    std::ifstream file;
    file.open(filepath);

    int width, height;
    file >> width >> height;

    std::string label;
    file >> label;
    if(label != "rooms:")
        return false;

    std::vector<Room> rooms;
    std::string line_str;
    for(std::getline(file >> std::ws, line_str); line_str != "tiles:" && !file.eof();
        std::getline(file >> std::ws, line_str))
    {
        std::stringstream line(line_str);
        std::string room_name;
        std::getline(line >> std::ws, room_name, ':');
        // TODO: Handle items

        rooms.emplace_back(room_name);
    }

    if(file.eof())
        return false;

    std::vector<int> indices;
    indices.reserve(width * height);
    for(std::getline(file >> std::ws, line_str); line_str != "collision:" && !file.eof();
        std::getline(file >> std::ws, line_str))
    {
        std::stringstream line(line_str);
        int idx;
        while(!line.eof())
        {
            line >> idx;
            indices.push_back(idx);
        }
    }

    if(file.eof() || indices.size() != width * height)
        return false;

    std::vector<std::array<bool, 2>> collision_map;
    collision_map.reserve(width * height);
    for(std::getline(file >> std::ws, line_str); !file.eof();
        std::getline(file >> std::ws, line_str))
    {
        std::stringstream line(line_str);
        bool east, south;
        while(!line.eof())
        {
            line >> east >> south;
            collision_map.push_back({east, south});
        }
    }

    if(collision_map.size() != width * height)
        return false;

    game_board = load_from_memory(width, height, rooms, indices, collision_map);
    return true;
}
