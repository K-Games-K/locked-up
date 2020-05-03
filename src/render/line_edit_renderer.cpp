#include "render/line_edit_renderer.hpp"

LineEditRenderer::LineEditRenderer(sf::RenderWindow& window)
    : WidgetRenderer(window), text_renderer(window) {}

void LineEditRenderer::render(const Ui::LineEdit& text_edit, const float dt,
    sf::Vector2f parent_pos, sf::Vector2f parent_size)
{
    if(!text_edit.is_enabled())
        return;

    time += dt * CURSOR_BLINK_SPEED;
    if(time >= 2)
        time -= 2;

    sf::Vector2f text_edit_pos = text_edit.get_global_position(parent_pos, parent_size);
    sf::Vector2f text_edit_size = text_edit.get_global_size(parent_size);
    sf::RectangleShape text_edit_rect(text_edit.get_global_size(parent_size));
    text_edit_rect.setPosition(text_edit_pos);
    text_edit_rect.setFillColor(text_edit.get_background_color());
    text_edit_rect.setOutlineColor(text_edit.get_border_color());
    text_edit_rect.setOutlineThickness(1);
    window.draw(text_edit_rect);

    const Ui::Text& text = text_edit.get_text();
    float text_width = text.get_local_size().x;
    float margin = text_edit.get_margin();
    if(text_edit.is_active() && time <= 1)
    {
        float font_size = (float) text.get_font_size();
        sf::RectangleShape cursor_rect({1, font_size});
        cursor_rect.setPosition(
            text_edit_pos + sf::Vector2f(text_width + margin, (text_edit_size.y - font_size) / 2)
        );
        cursor_rect.setFillColor(text.get_color());
        window.draw(cursor_rect);
    }

    text_renderer.render(text, dt, text_edit_pos + sf::Vector2f(margin, 0), text_edit_size);
}
