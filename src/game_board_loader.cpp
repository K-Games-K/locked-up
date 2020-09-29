#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include <iostream>

#include "game_board_loader.hpp"

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

        std::string item_name;
        std::vector<Item> items;
        while(!line.eof())
        {
            std::getline(line >> std::ws, item_name, ',');
            items.emplace_back(item_name, "", Item::Type::Prove);
        }

        rooms.emplace_back(room_name, items);
    }

    if(file.eof())
        return false;

    std::vector<int> tiles;
    tiles.reserve(width * height);
    for(std::getline(file >> std::ws, line_str); line_str != "collision:" && !file.eof();
        std::getline(file >> std::ws, line_str))
    {
        std::stringstream line(line_str);
        int idx;
        while(!line.eof())
        {
            line >> idx;
            tiles.push_back(idx);
        }
    }

    if(file.eof() || tiles.size() != width * height)
        return false;

    std::vector<std::array<bool, 2>> collision_map;
    collision_map.reserve(width * height);
    for(std::getline(file >> std::ws, line_str); line_str != "neighbours:" && !file.eof();
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

    std::vector<std::vector<int>> neighbours_map(rooms.size());
    for(std::getline(file >> std::ws, line_str); !file.eof();
        std::getline(file >> std::ws, line_str))
    {
        std::stringstream line(line_str);
        int room1, room2;
        while(!line.eof())
        {
            line >> room1 >> room2;
            neighbours_map[room1].push_back(room2);
            neighbours_map[room2].push_back(room1);
        }
    }

    game_board.width = width;
    game_board.height = height;
    game_board.rooms = std::move(rooms);
    game_board.tiles = std::move(tiles);
    game_board.collision_map = std::move(collision_map);
    game_board.neighbours_map = std::move(neighbours_map);
    return true;
}

void GameBoardLoader::load_from_packet(GameBoard& game_board, const Packet::GameBoardPacket& game_board_packet)
{
    std::vector<Room> rooms(game_board_packet.room_names().begin(), game_board_packet.room_names().end());
    std::vector<int> tiles(game_board_packet.tiles().begin(), game_board_packet.tiles().end());

    std::vector<std::array<bool, 2>> collision_map;
    collision_map.reserve(game_board_packet.collision_map_size() / 2);
    for(int i = 0; i < game_board_packet.collision_map_size(); i += 2)
    {
        collision_map.push_back({
            game_board_packet.collision_map(i),
            game_board_packet.collision_map(i + 1)
        });
    }

    game_board.width = game_board_packet.game_board_width();
    game_board.height = game_board_packet.game_board_height();
    game_board.rooms = std::move(rooms);
    game_board.tiles = std::move(tiles);
    game_board.collision_map = std::move(collision_map);
    game_board.neighbours_map.clear();
}

void GameBoardLoader::save_to_packet(const GameBoard& game_board, Packet::GameBoardPacket& game_board_packet)
{
    game_board_packet.Clear();

    game_board_packet.set_game_board_width(game_board.width);
    game_board_packet.set_game_board_height(game_board.height);

    for(auto& room : game_board.rooms)
    {
        game_board_packet.add_room_names(room.get_name());
    }

    *game_board_packet.mutable_tiles() = {game_board.tiles.begin(), game_board.tiles.end()};

    for(auto& entry : game_board.collision_map)
    {
        game_board_packet.add_collision_map(entry[0]);
        game_board_packet.add_collision_map(entry[1]);
    }
}
