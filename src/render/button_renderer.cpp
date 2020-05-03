#include "render/button_renderer.hpp"

ButtonRenderer::ButtonRenderer(sf::RenderWindow& window)
    : WidgetRenderer(window) {}

void ButtonRenderer::render(const Ui::Button& button, const float dt, sf::Vector2f parent_pos,
    sf::Vector2f parent_size)
{
    sf::Vector2f button_position = button.get_global_position(parent_pos, parent_size);
    sf::RectangleShape button_rect(button.get_global_size(parent_size));
    button_rect.setPosition(button_position);
    button_rect.setFillColor(button.get_color());
    window.draw(button_rect);
}

void ButtonRenderer::render(const Ui::TexturedButton& button, const float dt,
    sf::Vector2f parent_pos, sf::Vector2f parent_size)
{
    sf::Vector2f button_position = button.get_global_position(parent_pos, parent_size);
    sf::Sprite button_sprite(button.get_texture());
    button_sprite.setPosition(button_position);
    window.draw(button_sprite);
}
