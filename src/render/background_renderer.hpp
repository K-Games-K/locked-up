#pragma once

#include <SFML/Graphics.hpp>

#include "renderer.hpp"

class BackgroundRenderer : public Renderer<sf::Texture>
{
public:
    BackgroundRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const sf::Texture& background, const float dt) override;
};
