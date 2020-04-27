#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "ui/textured_button.hpp"

class TexturedButtonRenderer : public WidgetRenderer<Ui::TexturedButton>
{
private:
    TextRenderer text_renderer;

    sf::Sprite button_sprite;

public:
    TexturedButtonRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::TexturedButton& button, const float dt) override;
};
