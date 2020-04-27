#include "game_board_loader.hpp"
#include "network/packet/game_board_packet.hpp"

#include <array>

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
    int width, height;
    std::vector<Room> rooms;
    std::vector<int> indices;
    std::vector<std::array<bool, 2>> collision_map;
    GameBoardLoader::save_in_memory(
        game_board,
        width,
        height,
        rooms,
        indices,
        collision_map
    );

    data << width << height << rooms.size();
    for(auto& room : rooms)
        data << room.get_name();

    data << indices.size();
    for(auto& idx : indices)
        data << idx;

    data << collision_map.size();
    for(auto& collision : collision_map)
        data << collision[0] << collision[1];
}

void GameBoardPacket::deserialize(sf::Packet& data)
{
    int width, height;
    size_t rooms_size;
    std::vector<Room> rooms;
    data >> width >> height >> rooms_size;

    rooms.reserve(rooms_size);
    for(int i = 0; i < rooms_size; ++i)
    {
        std::string room_name;
        data >> room_name;
        rooms.emplace_back(room_name);
    }

    size_t indices_size;
    std::vector<int> indices;
    data >> indices_size;
    indices.reserve(indices_size);
    for(int i = 0; i < indices_size; ++i)
    {
        int idx;
        data >> idx;
        indices.push_back(idx);
    }

    size_t collision_map_size;
    std::vector<std::array<bool, 2>> collision_map;
    data >> collision_map_size;
    collision_map.reserve(collision_map_size);
    for(int i = 0; i < collision_map_size; ++i)
    {
        bool east, south;
        data >> east >> south;
        collision_map.push_back({east, south});
    }

    game_board = GameBoardLoader::load_from_memory(
        width,
        height,
        rooms,
        indices,
        collision_map
    );
}
