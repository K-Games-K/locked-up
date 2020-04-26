#include "render/player_renderer.hpp"

PlayerRenderer::PlayerRenderer(sf::RenderWindow& window, ResourceManagers resources)
        : CameraRenderer(window, resources)
{
    player_sprite.setTexture(resources.textures.get("player"));
}

void PlayerRenderer::render(const std::vector<Player>& players, const float dt)
{
    for(auto& player : players)
    {
        sf::Vector2f target_pos(
                (player.get_position().x - camera_pos.x) * TILE_SIZE,
                (player.get_position().y - camera_pos.y) * TILE_SIZE
        );

        player_sprite.setPosition(target_pos + game_board_pos);
        window.draw(player_sprite);
    }
}
