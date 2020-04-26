#include "render/textured_button_renderer.hpp"

TexturedButtonRenderer::TexturedButtonRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : WidgetRenderer(window, resources), button_renderer(window, resources)
{}

void TexturedButtonRenderer::render(const Ui::TexturedButton& button, const float dt)
{
    sf::Vector2f button_position = button.get_relative_position(origin_pos, parent_size);
    button_sprite.setTexture(button.get_texture());
    button_sprite.setPosition(button_position);
    window.draw(button_sprite);

    button_renderer.set_origin_pos(origin_pos);
    button_renderer.set_parent_size(parent_size);
    button_renderer.render(button, dt);
}
