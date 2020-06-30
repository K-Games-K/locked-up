#pragma once

#include "renderer.hpp"

/// Base class for renderers of widgets.
template<typename T>
class WidgetRenderer
{
protected:
    sf::RenderWindow& window;

public:
    WidgetRenderer(sf::RenderWindow& window)
        : window(window) {}

    virtual void
    render(const T& widget, const float dt, sf::Vector2f parent_pos, sf::Vector2f parent_size) = 0;
};
