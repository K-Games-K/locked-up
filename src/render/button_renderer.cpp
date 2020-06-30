#include "button_renderer.hpp"

ButtonRenderer::ButtonRenderer(sf::RenderWindow& window)
    : WidgetRenderer(window) {}

void ButtonRenderer::render(const Ui::Button& button, const float dt, sf::Vector2f parent_pos,
    sf::Vector2f parent_size)
{
    sf::Vector2f button_position = button.get_global_position(parent_pos, parent_size);

    if(button.has_texture())
    {
        auto& texture = button.get_texture();
        sf::Vector2f factors = {
            button.get_size().x / texture.getSize().x,
            button.get_size().y / texture.getSize().y
        };
        sf::Sprite button_sprite(texture);
        button_sprite.setPosition(button_position);
        button_sprite.setScale(factors);
        window.draw(button_sprite);
    }

    sf::RectangleShape button_rect(button.get_size());
    button_rect.setPosition(button_position);
    button_rect.setFillColor(button.get_color());
    window.draw(button_rect);
}
