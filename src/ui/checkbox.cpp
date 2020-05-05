#include "ui/checkbox.hpp"

namespace Ui
{
    Checkbox::Checkbox(bool checked)
        : checked(checked) {}

    bool Checkbox::handle_event(const sf::Event& event, sf::Vector2f mouse_pos,
        sf::Vector2f parent_pos, sf::Vector2f parent_size)
    {
        sf::Vector2f widget_pos = get_global_position(parent_pos, parent_size);
        sf::Vector2f widget_size = get_global_size(parent_size);
        sf::FloatRect widget_box(widget_pos, widget_size);

        if(event.type == sf::Event::MouseButtonPressed &&
            widget_box.contains(mouse_pos))
            checked = !checked;

        return Widget::handle_event(event, mouse_pos, widget_pos, widget_size);
    }

    Checkbox& Checkbox::set_checked(bool checked)
    {
        this->checked = checked;

        return *this;
    }

    bool Checkbox::is_checked() const
    {
        return checked;
    }

    Checkbox& Checkbox::set_background_color(Color background_color)
    {
        this->background_color = background_color;

        return *this;
    }

    Color Checkbox::get_background_color() const
    {
        return background_color;
    }

    Checkbox& Checkbox::set_outline_color(Color outline_color)
    {
        this->outline_color = outline_color;

        return *this;
    }

    Color Checkbox::get_outline_color() const
    {
        return outline_color;
    }

    Checkbox& Checkbox::set_outline_thickness(float outline_thickness)
    {
        this->outline_thickness = outline_thickness;

        return *this;
    }

    float Checkbox::get_outline_thickness() const
    {
        return outline_thickness;
    }

    Checkbox& Checkbox::set_tick_color(Color tick_color)
    {
        this->tick_color = tick_color;

        return *this;
    }

    Color Checkbox::get_tick_color() const
    {
        return tick_color;
    }

    Checkbox& Checkbox::set_tick_thickness(float tick_thickness)
    {
        this->tick_thickness = tick_thickness;

        return *this;
    }

    float Checkbox::get_tick_thickness() const
    {
        return tick_thickness;
    }

    Checkbox* Checkbox::clone() const
    {
        return new Checkbox(*this);
    }
}
