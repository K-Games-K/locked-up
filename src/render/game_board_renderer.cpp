#include "render/game_board_renderer.hpp"

GameBoardRenderer::GameBoardRenderer(sf::RenderWindow& window, ResourceManagers resources)
        : Renderer(window, resources)
{
    game_board_sprite.setTexture(resources.textures.get("mapa4"));
}

void GameBoardRenderer::render(const GameBoard& game_board, sf::Vector2f camera_pos)
{
    game_board_sprite.setPosition(game_board_position);
    game_board_sprite.setTextureRect(
            {
                    (int) (camera_pos.x * TILE_SIZE), (int) (camera_pos.y * TILE_SIZE),
                    (int) game_board_size.x, (int) game_board_size.y
            }
    );

    window.draw(game_board_sprite);
}

void GameBoardRenderer::set_game_board_position(sf::Vector2f game_board_position)
{
    this->game_board_position = game_board_position;
}

void GameBoardRenderer::set_game_board_size(sf::Vector2f game_board_size)
{
    this->game_board_size = game_board_size;
}
