#include "render/textured_button_renderer.hpp"

TexturedButtonRenderer::TexturedButtonRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : WidgetRenderer(window, resources), text_renderer(window, resources)
{}

void TexturedButtonRenderer::render(const Ui::TexturedButton& button, const float dt)
{
    sf::Vector2f button_position = button.get_relative_position(origin_pos, parent_size);
    button_sprite.setTexture(button.get_texture());
    button_sprite.setPosition(button_position);
    window.draw(button_sprite);

    text_renderer.set_origin_pos(button_position);
    text_renderer.set_parent_size(button.get_size());
    text_renderer.render(button.get_text(), dt);
}
