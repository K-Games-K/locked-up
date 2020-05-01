#include <array>

#include "game_board_loader.hpp"
#include "network/packet/game_board_packet.hpp"

GameBoardPacket::GameBoardPacket()
    : Packet(PACKET_ID)
{}

GameBoardPacket::GameBoardPacket(GameBoard& game_board)
    : Packet(PACKET_ID), game_board(game_board)
{}

GameBoard GameBoardPacket::get_game_board() const
{
    return game_board;
}

void GameBoardPacket::serialize(sf::Packet& data) const
{
    data << game_board.width << game_board.height << (uint64_t)game_board.rooms.size();
    for(auto& room : game_board.rooms)
        data << room.get_name();

    data << (uint64_t) game_board.tiles.size();
    for(auto& idx : game_board.tiles)
        data << idx;

    data << (uint64_t) game_board.collision_map.size();
    for(auto& collision : game_board.collision_map)
        data << collision[0] << collision[1];
}

void GameBoardPacket::deserialize(sf::Packet& data)
{
    int width, height;
    uint64_t rooms_size;
    std::vector<Room> rooms;
    data >> width >> height >> rooms_size;

    rooms.reserve(rooms_size);
    for(int i = 0; i < rooms_size; ++i)
    {
        std::string room_name;
        data >> room_name;
        rooms.emplace_back(room_name);
    }

    uint64_t indices_size;
    std::vector<int> tiles;
    data >> indices_size;
    tiles.reserve(indices_size);
    for(int i = 0; i < indices_size; ++i)
    {
        int idx;
        data >> idx;
        tiles.push_back(idx);
    }

    uint64_t collision_map_size;
    std::vector<std::array<bool, 2>> collision_map;
    data >> collision_map_size;
    collision_map.reserve(collision_map_size);
    for(int i = 0; i < collision_map_size; ++i)
    {
        bool east, south;
        data >> east >> south;
        collision_map.push_back({east, south});
    }

    game_board.width = width;
    game_board.height = height;
    game_board.rooms = std::move(rooms);
    game_board.tiles = std::move(tiles);
    game_board.collision_map = std::move(collision_map);
    game_board.neighbours_map = {};
}
