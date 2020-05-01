#include "render/player_renderer.hpp"
#include "ui/text.hpp"

PlayerRenderer::PlayerRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : CameraRenderer(window, resources), text_renderer(window, resources)
{}

void PlayerRenderer::render(const std::vector<Player>& players, const float dt)
{
    sf::FloatRect game_board_rect({0, 0}, game_board_size);

    for(auto& player : players)
    {
        sf::Vector2f player_pos(
            (player.get_position().x - camera_pos.x + 0.5) * TILE_SIZE,
            (player.get_position().y - camera_pos.y + 0.5) * TILE_SIZE
        );

        if(!game_board_rect.contains(player_pos))
            continue;

        auto& texture = resources.textures.get(player.get_avatar());
        sf::Sprite player_sprite(texture);
        player_sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y - TILE_SIZE / 2);
        player_sprite.setPosition(player_pos + game_board_pos);
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

        auto& texture = resources.textures.get(player.get_avatar());
        sf::Vector2f player_size(texture.getSize());
        player_pos += sf::Vector2f(TILE_SIZE / 2, TILE_SIZE) -
            sf::Vector2f(player_size.x / 2, texture.getSize().y);

        Ui::Text nickname(
            player.get_nickname(),
            resources.fonts.get("IndieFlower-Regular"),
            {0, 0},
            {sf::Color::White, 20, sf::Color::Black, 1},
            Ui::Anchor::CenterTop, Ui::Anchor::CenterBottom
        );

        text_renderer.set_origin_pos(player_pos + game_board_pos);
        text_renderer.set_parent_size(player_size);
        text_renderer.render(nickname, dt);
    }
}
