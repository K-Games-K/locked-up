#pragma once

#include <SFML/Graphics.hpp>

#include "resource_manager.hpp"

struct ResourceManagers
{
    ResourceManager<sf::Texture>& textures;
    ResourceManager<sf::Font>& fonts;
};

template<typename T>
class Renderer
{
protected:
    sf::RenderWindow& window;
    ResourceManagers resources;

public:
    Renderer(sf::RenderWindow& window, ResourceManagers resources)
            : window(window), resources(resources)
    {};

    virtual ~Renderer() = default;

    virtual void render(const T& obj, sf::Vector2f camera_pos) = 0;
};
