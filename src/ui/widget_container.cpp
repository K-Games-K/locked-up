#include "ui/widget_container.hpp"

namespace Ui
{
    WidgetContainer::WidgetContainer(sf::Vector2f position, sf::Vector2f size, Anchor origin,
        Anchor anchor) : Widget(position, size, origin, anchor)
    {}
}
