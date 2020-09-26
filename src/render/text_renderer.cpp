#include "render/text_renderer.hpp"

TextRenderer::TextRenderer(sf::RenderWindow& window)
    : WidgetRenderer(window) {}

void TextRenderer::render(const Ui::Text& text, const float dt, sf::Vector2f parent_pos,
    sf::Vector2f parent_size)
{
    sf::Text sf_text(text.get_string(), text.get_font(), text.get_font_size());
    sf::Vector2f text_position = text.get_global_position(parent_pos, parent_size);

    sf_text.setPosition(text_position);
    sf_text.setFillColor(text.get_color());
    sf_text.setOutlineThickness(text.get_outline_thickness());
    sf_text.setOutlineColor(text.get_outline_color());
    sf_text.setStyle(text.get_style());
    window.draw(sf_text);
}
