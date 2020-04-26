#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#include "widget.hpp"

namespace Ui
{
    class UserInterface : public Widget
    {
    private:
        sf::RenderWindow& window;
        std::vector<Widget*> widgets;

    public:
        UserInterface(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size);

        ~UserInterface();

        void handle_event(sf::Event event);

        void add_widget(Widget* widget);

        void remove_widget(Widget* widget);

        const std::vector<Widget*>& get_widgets() const;
    };
}
