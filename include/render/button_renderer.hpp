#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "ui/button.hpp"

class ButtonRenderer : public WidgetRenderer<Ui::Button>
{
private:
    TextRenderer text_renderer;

public:
    ButtonRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::Button& button, const float dt) override;
};

