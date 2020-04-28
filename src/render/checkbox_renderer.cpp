#include "utils.hpp"
#include "render/checkbox_renderer.hpp"

CheckboxRender::CheckboxRender(sf::RenderWindow& window, ResourceManagers resources)
    : WidgetRenderer(window, resources)
{}

void CheckboxRender::render(const Ui::Checkbox& checkbox, const float dt)
{
    if(!checkbox.is_enabled())
        return;

    sf::Vector2f checkbox_pos = checkbox.get_relative_position(origin_pos, parent_size);
    Ui::CheckboxSettings checkbox_settings = checkbox.get_settings();
    sf::RectangleShape checkbox_rect(checkbox.get_size());
    checkbox_rect.setPosition(checkbox_pos);
    checkbox_rect.setFillColor(checkbox_settings.background_color);
    checkbox_rect.setOutlineColor(checkbox_settings.outline_color);
    checkbox_rect.setOutlineThickness(checkbox_settings.outline_thickness);
    window.draw(checkbox_rect);

    if(checkbox.is_checked())
    {
        sf::RectangleShape tick_rect(
            {
                checkbox_settings.tick_thickness,
                Utils::min(checkbox.get_size().x, checkbox.get_size().y)
            }
        );
        tick_rect.setOrigin(tick_rect.getSize() / 2.0f);
        tick_rect.setPosition(checkbox_pos + checkbox_rect.getSize() / 2.0f);
        tick_rect.setFillColor(checkbox_settings.tick_color);
        tick_rect.setRotation(45);
        window.draw(tick_rect);
        tick_rect.setRotation(-45);
        window.draw(tick_rect);
    }
}

void CheckboxRender::render(const Ui::TexturedCheckbox& checkbox, const float dt)
{
    if(!checkbox.is_enabled())
        return;

    sf::Vector2f checkbox_pos = checkbox.get_relative_position(origin_pos, parent_size);
    checkbox_sprite.setPosition(checkbox_pos);
    checkbox_sprite.setTexture(checkbox.get_texture());
    window.draw(checkbox_sprite);
}


