#include "render/player_renderer.hpp"
#include "ui/text.hpp"

PlayerRenderer::PlayerRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : CameraRenderer(window, resources), text_renderer(window, resources)
{
    player_sprite.setTexture(resources.textures.get("player"));
}

void PlayerRenderer::render(const std::vector<Player>& players, const float dt)
{
    for(auto& player : players)
    {
        sf::Vector2f player_pos(
            (player.get_position().x - camera_pos.x) * TILE_SIZE,
            (player.get_position().y - camera_pos.y) * TILE_SIZE
        );

        player_sprite.setPosition(player_pos + game_board_pos);
        window.draw(player_sprite);
    }

    for(auto& player : players)
    {
        Ui::Text nickname(
            player.get_nickname(),
            resources.fonts.get("IndieFlower-Regular"),
            18,
            {0, 0},
            sf::Color::White,
            Ui::Anchor::CenterTop, Ui::Anchor::CenterBottom
        );

        sf::Vector2f player_pos(
            (player.get_position().x - camera_pos.x) * TILE_SIZE,
            (player.get_position().y - camera_pos.y) * TILE_SIZE
        );
        sf::FloatRect player_bounds = player_sprite.getLocalBounds();
        sf::Vector2f player_size(
            player_bounds.width + player_bounds.left,
            player_bounds.height + player_bounds.top
        );

        text_renderer.set_origin_pos(player_pos + game_board_pos);
        text_renderer.set_parent_size(player_size);
        text_renderer.render(nickname, dt);
    }
}
