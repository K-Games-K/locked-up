#pragma once

#include "widget.hpp"

namespace Ui
{
    struct CheckboxSettings
    {
        sf::Color background_color = sf::Color::Black;
        sf::Color outline_color = sf::Color::White;
        float outline_thickness = 1;
        sf::Color tick_color = sf::Color::White;
        float tick_thickness = 2;
    };

    class Checkbox : public Widget
    {
    protected:
        bool checked = false;

    private:
        CheckboxSettings settings;

    public:
        Checkbox(sf::Vector2f position = {0, 0}, sf::Vector2f size = {0, 0},
            CheckboxSettings settings = CheckboxSettings(), Anchor origin = Anchor::TopLeft,
            Anchor anchor = Anchor::TopLeft);

        void handle_event(sf::Event event, sf::Vector2f mouse_pos) override;

        bool is_checked() const;

        void set_background_color(sf::Color background_color);

        void set_outline_color(sf::Color outline_color);

        void set_outline_thickness(float outline_thickness);

        void set_tick_color(sf::Color tick_color);

        void set_tick_thickness(float tick_thickness);

        void set_settings(CheckboxSettings settings);

        CheckboxSettings get_settings() const;
    };
}
