#include "utils.hpp"
#include "render/checkbox_renderer.hpp"

CheckboxRenderer::CheckboxRenderer(sf::RenderWindow& window)
    : WidgetRenderer(window) {}

void CheckboxRenderer::render(const Ui::Checkbox& checkbox, const float dt, sf::Vector2f parent_pos,
    sf::Vector2f parent_size)
{
    sf::Vector2f checkbox_pos = checkbox.get_global_position(parent_pos, parent_size);
    sf::RectangleShape checkbox_rect(checkbox.get_global_size(parent_size));
    checkbox_rect.setPosition(checkbox_pos);
    checkbox_rect.setFillColor(checkbox.get_background_color());
    checkbox_rect.setOutlineColor(checkbox.get_outline_color());
    checkbox_rect.setOutlineThickness(checkbox.get_outline_thickness());
    window.draw(checkbox_rect);

    if(checkbox.is_checked())
    {
        sf::RectangleShape tick_rect(
            {
                checkbox.get_tick_thickness(),
                Utils::min(checkbox_rect.getSize().x, checkbox_rect.getSize().y)
            }
        );
        tick_rect.setOrigin(tick_rect.getSize() / 2.0f);
        tick_rect.setPosition(checkbox_pos + checkbox_rect.getSize() / 2.0f);
        tick_rect.setFillColor(checkbox.get_tick_color());
        tick_rect.setRotation(45);
        window.draw(tick_rect);
        tick_rect.setRotation(-45);
        window.draw(tick_rect);
    }
}


