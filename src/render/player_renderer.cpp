#include "render/player_renderer.hpp"

PlayerRenderer::PlayerRenderer(sf::RenderWindow& window, ResourceManagers resources)
        : Renderer(window, resources)
{
    player_sprite.setTexture(resources.textures.get("player"));
}

void PlayerRenderer::render(const std::vector<Player>& players, sf::Vector2f camera_pos)
{
    for(auto& player : players)
    {
        sf::Vector2f target_pos(
                (player.get_position().x - camera_pos.x) * TILE_SIZE,
                (player.get_position().y - camera_pos.y) * TILE_SIZE
        );

        player_sprite.setPosition(target_pos + game_board_position);
        window.draw(player_sprite);
    }
}

void PlayerRenderer::set_game_board_position(sf::Vector2f game_board_position)
{
    this->game_board_position = game_board_position;
}
