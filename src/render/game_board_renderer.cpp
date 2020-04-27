#include "render/game_board_renderer.hpp"

GameBoardRenderer::GameBoardRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : CameraRenderer(window, resources)
{
    game_board_sprite.setTexture(resources.textures.get("mapa4"));
}

void GameBoardRenderer::render(const GameBoard& game_board, const float dt)
{
    game_board_sprite.setPosition(game_board_pos);
    game_board_sprite.setTextureRect(
        {
            (int) (camera_pos.x * TILE_SIZE), (int) (camera_pos.y * TILE_SIZE),
            (int) game_board_size.x, (int) game_board_size.y
        }
    );

    window.draw(game_board_sprite);
}
