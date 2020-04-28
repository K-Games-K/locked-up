#include "render/button_renderer.hpp"

ButtonRenderer::ButtonRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : WidgetRenderer(window, resources), text_renderer(window, resources)
{}

void ButtonRenderer::render(const Ui::Button& button, const float dt)
{
    if(!button.is_enabled())
        return;

    sf::Vector2f button_position = button.get_relative_position(origin_pos, parent_size);
    sf::RectangleShape button_rect(button.get_size());
    button_rect.setPosition(button_position);
    button_rect.setFillColor(button.get_color());
    window.draw(button_rect);

    text_renderer.set_origin_pos(button_position);
    text_renderer.set_parent_size(button.get_size());
    text_renderer.render(button.get_text(), dt);
}

void ButtonRenderer::render(const Ui::TexturedButton& button, const float dt)
{
    if(!button.is_enabled())
        return;

    sf::Vector2f button_position = button.get_relative_position(origin_pos, parent_size);
    sf::Sprite button_sprite(button.get_texture());
    button_sprite.setPosition(button_position);
    window.draw(button_sprite);

    text_renderer.set_origin_pos(button_position);
    text_renderer.set_parent_size(button.get_size());
    text_renderer.render(button.get_text(), dt);
}
