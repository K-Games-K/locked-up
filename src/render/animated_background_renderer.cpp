#define _USE_MATH_DEFINES
#include <cmath>

#include "animated_background_renderer.hpp"
#include "../utils.hpp"

AnimatedBackgroundRenderer::AnimatedBackgroundRenderer(sf::RenderWindow& window,
    ResourceManagers resources) : Renderer(window, resources)
{}

void AnimatedBackgroundRenderer::render(const sf::Texture& background, const float dt)
{
    time += dt * speed;
    if(time >= 2 * M_PI)
        time -= 2 * M_PI;

    sf::Vector2f offset = {std::cos(time), std::sin(time)};
    offset *= radius;

    sf::Sprite bg_sprite(background);

    sf::Vector2f texture_size = (sf::Vector2f) background.getSize();
    sf::Vector2f window_size = (sf::Vector2f) window.getSize();
    float scale = Utils::max(
        (window_size.x + 2.1 * radius) / texture_size.x,
        (window_size.y + 2.1 * radius) / texture_size.y
    );
    bg_sprite.setScale(scale, scale);

    sf::Vector2f position = window_size / 2.0f - texture_size * scale / 2.0f;

    bg_sprite.setPosition(position + offset);
    window.draw(bg_sprite);
}
