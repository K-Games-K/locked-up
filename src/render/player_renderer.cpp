#define _USE_MATH_DEFINES
#include <cmath>

#include "render/player_renderer.hpp"
#include "ui/text.hpp"

PlayerRenderer::PlayerRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : CameraRenderer(window, resources), text_renderer(window)
{}

void PlayerRenderer::render(const std::vector<Player>& players, const float dt)
{
    time += dt * speed;
    if(time >= 2 * M_PI)
        time -= 2 * M_PI;

    sf::FloatRect game_board_rect({0, 0}, game_board_size);

    for(auto& player : players)
    {
        sf::Vector2f player_pos(
            (player.get_position().x - camera_pos.x + 0.5) * TILE_SIZE,
            (player.get_position().y - camera_pos.y + 0.5) * TILE_SIZE
        );

        if(!game_board_rect.contains(player_pos))
            continue;

        auto& texture = resources.textures.get(player.get_avatar_name());
        sf::Sprite player_sprite(texture);
        player_sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y - TILE_SIZE / 2);
        player_sprite.setPosition(player_pos + game_board_pos);
        player_sprite.setScale(1.0, 0.05 * sin(time) + 1.0);
        window.draw(player_sprite);
    }

    for(auto& player : players)
    {
        sf::Vector2f player_pos(
            (player.get_position().x - camera_pos.x) * TILE_SIZE,
            (player.get_position().y - camera_pos.y) * TILE_SIZE
        );

        if(!game_board_rect.contains(player_pos))
            continue;

        auto& texture = resources.textures.get(player.get_avatar_name());
        sf::Vector2f player_size(texture.getSize());
        player_pos += sf::Vector2f(TILE_SIZE / 2, TILE_SIZE) -
            sf::Vector2f(player_size.x / 2, texture.getSize().y);

        Ui::Text nickname_text(resources.fonts.get("IndieFlower-Regular"), player.get_nickname());
        nickname_text.set_color(Ui::Color::White)
            .set_font_size(20)
            .set_outline_color(Ui::Color::Black)
            .set_outline_thickness(1)
            .set_origin(Ui::Origin::CenterTop)
            .set_anchor(Ui::Anchor::CenterBottom);

        text_renderer.render(nickname_text, dt, player_pos + game_board_pos, player_size);
    }
}
