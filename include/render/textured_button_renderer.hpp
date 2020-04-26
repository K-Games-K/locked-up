#pragma once

#include "renderer.hpp"
#include "button_renderer.hpp"
#include "ui/textured_button.hpp"

class TexturedButtonRenderer : public Renderer<Ui::TexturedButton>
{
private:
    ButtonRenderer button_renderer;

public:
    TexturedButtonRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::TexturedButton& button, sf::Vector2f origin = {0, 0}) override;
};
