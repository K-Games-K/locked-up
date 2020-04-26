#include "render/text_renderer.hpp"

TextRenderer::TextRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : WidgetRenderer(window, resources)
{}

void TextRenderer::render(const Ui::Text& text, const float dt)
{
    if(!text.is_enabled())
        return;

    sf::Text sf_text(text.get_string(), text.get_font(), text.get_font_size());
    sf::Vector2f text_position = text.get_relative_position(origin_pos, parent_size);

    sf_text.setPosition(text_position);
    sf_text.setFillColor(text.get_color());
    window.draw(sf_text);
}
