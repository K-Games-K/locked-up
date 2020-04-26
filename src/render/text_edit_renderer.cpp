#include "render/text_edit_renderer.hpp"

TextEditRenderer::TextEditRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : WidgetRenderer(window, resources), text_renderer(window, resources)
{}

void TextEditRenderer::render(const Ui::TextEdit& text_edit, const float dt)
{
    sf::Vector2f text_edit_pos = text_edit.get_relative_position(origin_pos, parent_size);
    Ui::TextEditColors colors = text_edit.get_colors();
    sf::RectangleShape text_edit_rect(text_edit.get_size());
    text_edit_rect.setPosition(text_edit_pos);
    text_edit_rect.setFillColor(colors.background_color);
    text_edit_rect.setOutlineColor(colors.border_color);
    text_edit_rect.setOutlineThickness(1);
    window.draw(text_edit_rect);

    text_renderer.set_origin_pos(text_edit_pos);
    text_renderer.set_parent_size(text_edit.get_size());
    text_renderer.render(text_edit.get_text(), dt);
}
