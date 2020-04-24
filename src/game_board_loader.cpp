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

GameBoard GameBoardLoader::load_from_file(const std::string& filepath)
{
    return GameBoard();
}
