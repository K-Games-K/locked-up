#define _USE_MATH_DEFINES
#include <cmath>

#include "render/main_menu_background_renderer.hpp"

MainMenuBackgroundRenderer::MainMenuBackgroundRenderer(sf::RenderWindow& window,
    ResourceManagers resources) : Renderer(window, resources)
{}

void MainMenuBackgroundRenderer::render(const sf::Texture& background, const float dt)
{
    time += dt * 0.2;
    if(time >= 2 * M_PI)
        time -= 2 * M_PI;

    sf::Vector2f offset = {std::cos(time), std::sin(time)};
    offset *= 200.0f;
    offset -= (sf::Vector2f) window.getSize() * 0.5f;

    sf::Sprite bg_sprite(background);
    bg_sprite.setScale(1.3, 1.3);
    bg_sprite.setPosition(offset);
    window.draw(bg_sprite);
}
