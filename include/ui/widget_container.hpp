#pragma once

#include "widget.hpp"

namespace Ui
{
    class WidgetContainer : public Widget
    {
    protected:
        WidgetContainer(sf::Vector2f position = {0, 0}, sf::Vector2f size = {0, 0},
            Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

    public:
        virtual void handle_event(sf::Event event, sf::Vector2f mouse_pos) = 0;

        virtual void update(const float dt) = 0;
    };
}
