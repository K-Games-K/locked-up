#include "utils.hpp"
#include "render/background_renderer.hpp"

BackgroundRenderer::BackgroundRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : Renderer(window, resources)
{}

void BackgroundRenderer::render(const sf::Texture& background, const float dt)
{
    sf::Sprite bg_sprite(background);

    sf::Vector2f texture_size = (sf::Vector2f) background.getSize();
    sf::Vector2f window_size = (sf::Vector2f) window.getSize();
    float scale = Utils::max(window_size.x / texture_size.x, window_size.y / texture_size.y);
    bg_sprite.setScale(scale, scale);

    sf::Vector2f position = window_size / 2.0f - texture_size * scale / 2.0f;

    bg_sprite.setPosition(position);
    window.draw(bg_sprite);
}
