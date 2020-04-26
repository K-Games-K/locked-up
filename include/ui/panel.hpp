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
        std::vector<Widget*> widgets;

    public:
        Panel(sf::Vector2f position, sf::Vector2f size);

        ~Panel();

        void handle_event(sf::Event event, sf::Vector2f mouse_pos) override;

        void add_widget(Widget* widget);

        void remove_widget(Widget* widget);

        const std::vector<Widget*>& get_widgets() const;
    };
}
