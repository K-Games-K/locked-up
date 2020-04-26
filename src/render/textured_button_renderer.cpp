#include "render/textured_button_renderer.hpp"

TexturedButtonRenderer::TexturedButtonRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : Renderer(window, resources), button_renderer(window, resources)
{}

void TexturedButtonRenderer::render(const Ui::TexturedButton& button, sf::Vector2f origin)
{
    sf::Vector2f button_position = origin + button.get_absolute_position();
    sf::Sprite button_sprite(button.get_texture());
    button_sprite.setPosition(button_position);
    window.draw(button_sprite);

    button_renderer.render(button, origin);
}
