#pragma once

#include <SFML/Graphics.hpp>

#include "renderer.hpp"

class MainMenuBackgroundRenderer : public Renderer<sf::Texture>
{
private:
    float time = 0;

public:
    MainMenuBackgroundRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const sf::Texture& background, const float dt);
};
