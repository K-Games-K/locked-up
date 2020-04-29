#include "ui/checkbox.hpp"

namespace Ui
{
    Checkbox::Checkbox(sf::Vector2f position, sf::Vector2f size, CheckboxSettings settings,
        Anchor origin, Anchor anchor)
        : Widget(position, size, origin, anchor),
        settings(settings)
    {}

    void Checkbox::handle_event(sf::Event event, sf::Vector2f mouse_pos)
    {
        sf::FloatRect widget_box({0, 0}, get_size());

        if(event.type == sf::Event::MouseButtonPressed &&
            widget_box.contains(mouse_pos))
            checked = !checked;
    }

    bool Checkbox::is_checked() const
    {
        return checked;
    }

    void Checkbox::set_background_color(sf::Color background_color)
    {
        settings.background_color = background_color;
    }

    void Checkbox::set_outline_color(sf::Color outline_color)
    {
        settings.outline_color = outline_color;
    }

    void Checkbox::set_outline_thickness(float outline_thickness)
    {
        settings.outline_thickness = outline_thickness;
    }

    void Checkbox::set_tick_color(sf::Color tick_color)
    {
        settings.tick_color = tick_color;
    }

    void Checkbox::set_tick_thickness(float tick_thickness)
    {
        settings.tick_thickness = tick_thickness;
    }

    void Checkbox::set_settings(CheckboxSettings settings)
    {
        this->settings = settings;
    }

    CheckboxSettings Checkbox::get_settings() const
    {
        return settings;
    }
}
