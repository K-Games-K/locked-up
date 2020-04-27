#pragma once

#include "widget_renderer.hpp"
#include "ui/widget.hpp"
#include "ui/text.hpp"

class TextRenderer : public WidgetRenderer<Ui::Text>
{
public:
    TextRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::Text& text, const float dt) override;
};
