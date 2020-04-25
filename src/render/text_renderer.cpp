#include "render/text_renderer.hpp"

TextRenderer::TextRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : Renderer(window, resources)
{}

void TextRenderer::render(const Ui::Text& text, sf::Vector2f origin)
{
    sf::Text sf_text(text.get_text(), text.get_font(), text.get_font_size());
    sf::Vector2f text_position = origin + text.get_absolute_position();

    sf_text.setPosition(text_position);
    sf_text.setFillColor(text.get_color());
    window.draw(sf_text);
}
