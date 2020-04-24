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
    data << game_board.get_width() << game_board.get_height();

    std::vector<Room> rooms;
    std::vector<int> indices;
    game_board.save_indices(rooms, indices);

    data << rooms.size();
    for(auto& room : rooms)
        data << room.get_name();

    data << indices.size();
    for(auto& idx : indices)
        data << idx;
}

void GameBoardPacket::deserialize(sf::Packet& data)
{
    int width, height;
    data >> width >> height;

    size_t rooms_size;
    std::vector<Room> rooms;
    data >> rooms_size;
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

    game_board.load_from_indices(width, height, rooms, indices);
}
