#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#include "widget.hpp"

namespace Ui
{
    class Panel : public Widget
    {
    private:
        sf::Color background_color;

        std::vector<Widget*> widgets;

    public:
        Panel(sf::Vector2f position = {0, 0}, sf::Vector2f size = {0, 0},
            sf::Color background_color = sf::Color::Transparent,
            Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

        ~Panel();

        void handle_event(sf::Event event, sf::Vector2f mouse_pos) override;

        void update(const float dt) override;

        void add_widget(Widget* widget);

        void remove_widget(Widget* widget);

        const std::vector<Widget*>& get_widgets() const;

        void set_background_color(sf::Color background_color);

        sf::Color get_background_color() const;

        void reset() override;
    };
}
