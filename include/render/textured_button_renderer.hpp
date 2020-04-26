#pragma once

#include "widget_renderer.hpp"
#include "button_renderer.hpp"
#include "ui/textured_button.hpp"

class TexturedButtonRenderer : public WidgetRenderer<Ui::TexturedButton>
{
private:
    ButtonRenderer button_renderer;

    sf::Sprite button_sprite;

public:
    TexturedButtonRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::TexturedButton& button, const float dt) override;
};
