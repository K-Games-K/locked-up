#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "../ui/line_edit.hpp"

class LineEditRenderer : public WidgetRenderer<Ui::LineEdit>
{
private:
    const float CURSOR_BLINK_SPEED = 0.5;

    TextRenderer text_renderer;

    float time = 0;

public:
    LineEditRenderer(sf::RenderWindow& window);

    void render(const Ui::LineEdit& text_edit, const float dt, sf::Vector2f parent_pos,
        sf::Vector2f parent_size) override;
};
