#include "render/text_edit_renderer.hpp"

TextEditRenderer::TextEditRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : WidgetRenderer(window, resources), text_renderer(window, resources)
{}

void TextEditRenderer::render(const Ui::TextEdit& text_edit, const float dt)
{
    if(!text_edit.is_enabled())
        return;

    time += dt * CURSOR_BLINK_SPEED;
    if(time >= 2)
        time -= 2;

    sf::Vector2f text_edit_pos = text_edit.get_relative_position(origin_pos, parent_size);
    Ui::TextEditColors colors = text_edit.get_colors();
    sf::RectangleShape text_edit_rect(text_edit.get_size());
    text_edit_rect.setPosition(text_edit_pos);
    text_edit_rect.setFillColor(colors.background_color);
    text_edit_rect.setOutlineColor(colors.border_color);
    text_edit_rect.setOutlineThickness(1);
    window.draw(text_edit_rect);

    const Ui::Text& text = text_edit.get_text();
    sf::Vector2f text_pos = text.get_relative_position(text_edit_pos, text_edit.get_size());
    float text_width = text.get_size().x;

    if(text_edit.is_active() && time <= 1)
    {
        sf::RectangleShape cursor_rect({1, (float) text.get_settings().font_size});
        cursor_rect.setPosition(text_pos + sf::Vector2f(text_width + 3, 0));
        cursor_rect.setFillColor(text.get_settings().color);
        window.draw(cursor_rect);
    }

    text_renderer.set_origin_pos(text_edit_pos);
    text_renderer.set_parent_size(text_edit.get_size());
    text_renderer.render(text, dt);
}
