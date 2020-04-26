#pragma once

#include "renderer.hpp"

/// Base class for renderers of widgets.
template<typename T>
class WidgetRenderer : public Renderer<T>
{
protected:
    sf::Vector2f origin_pos;
    sf::Vector2f parent_size;

public:
    WidgetRenderer(sf::RenderWindow& window, ResourceManagers resources)
        : Renderer<T>(window, resources)
    {}

    void set_origin_pos(sf::Vector2f origin_pos)
    {
        this->origin_pos = origin_pos;
    }

    void set_parent_size(sf::Vector2f parent_size)
    {
        this->parent_size = parent_size;
    }
};
