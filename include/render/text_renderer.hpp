#pragma once

#include "renderer.hpp"
#include "ui/widget.hpp"
#include "ui/text.hpp"

class TextRenderer : public Renderer<Ui::Text>
{
public:
    TextRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::Text& text, sf::Vector2f origin = {0, 0}) override;
};
