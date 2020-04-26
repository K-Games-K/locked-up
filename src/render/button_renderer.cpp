#include "render/button_renderer.hpp"

ButtonRenderer::ButtonRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : Renderer(window, resources), text_renderer(window, resources)
{}

void ButtonRenderer::render(const Ui::Button& button, sf::Vector2f origin)
{
    sf::Vector2f button_position = origin + button.get_absolute_position();
    sf::RectangleShape button_rect(button.get_size());
    button_rect.setPosition(button_position);
    button_rect.setFillColor(button.get_color());
    window.draw(button_rect);

    text_renderer.render(button.get_text(), origin);
}

