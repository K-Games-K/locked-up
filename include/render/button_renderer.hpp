#pragma once

#include "renderer.hpp"
#include "text_renderer.hpp"
#include "ui/button.hpp"

class ButtonRenderer : public Renderer<Ui::Button>
{
private:
    TextRenderer text_renderer;

public:
    ButtonRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::Button& button, sf::Vector2f origin = {0, 0}) override;
};

