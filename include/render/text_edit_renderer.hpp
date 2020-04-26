#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "ui/text_edit.hpp"

class TextEditRenderer : public WidgetRenderer<Ui::TextEdit>
{
private:
    const float CURSOR_BLINK_SPEED = 0.5;

    TextRenderer text_renderer;

    float time = 0;

public:
    TextEditRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::TextEdit& text_edit, const float dt) override;
};
