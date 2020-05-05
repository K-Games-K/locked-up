#pragma once

#include "widget.hpp"

namespace Ui
{
    class Checkbox : public Widget
    {
    public:
        Checkbox(bool checked = false);

        bool handle_event(const sf::Event& event, sf::Vector2f mouse_pos,
            sf::Vector2f parent_pos, sf::Vector2f parent_size) override;

        Checkbox& set_checked(bool checked);

        bool is_checked() const;

        Checkbox& set_background_color(Color background_color);

        Color get_background_color() const;

        Checkbox& set_outline_color(Color outline_color);

        Color get_outline_color() const;

        Checkbox& set_outline_thickness(float outline_thickness);

        float get_outline_thickness() const;

        Checkbox& set_tick_color(Color tick_color);

        Color get_tick_color() const;

        Checkbox& set_tick_thickness(float tick_thickness);

        float get_tick_thickness() const;

    protected:
        bool checked;

    private:
        Color background_color = Color::Black;
        Color outline_color = Color::White;
        float outline_thickness = 1;
        Color tick_color = Color::White;
        float tick_thickness = 2;

        Checkbox* clone() const override;
    };
}
