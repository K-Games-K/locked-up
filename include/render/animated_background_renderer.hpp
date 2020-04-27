#pragma once

#include <SFML/Graphics.hpp>

#include "renderer.hpp"

class AnimatedBackgroundRenderer : public Renderer<sf::Texture>
{
private:
    float time = 0;

public:
    AnimatedBackgroundRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const sf::Texture& background, const float dt);
};
