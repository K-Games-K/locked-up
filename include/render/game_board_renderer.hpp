#pragma once

#include "renderer.hpp"
#include "game_board.hpp"

class GameBoardRenderer : public Renderer<GameBoard>
{
private:
    const float TILE_SIZE = 40;

    sf::Sprite game_board_sprite;
    sf::Vector2f game_board_position;
    sf::Vector2f game_board_size;

public:
    GameBoardRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const GameBoard& game_board, sf::Vector2f camera_pos) override;

    void set_game_board_position(sf::Vector2f game_board_position);
    void set_game_board_size(sf::Vector2f game_board_size);
};
