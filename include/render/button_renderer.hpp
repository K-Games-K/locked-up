#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "ui/button.hpp"
#include "ui/textured_button.hpp"

class ButtonRenderer : public WidgetRenderer<Ui::Button>
{
public:
    ButtonRenderer(sf::RenderWindow& window);

    void render(const Ui::Button& button, const float dt, sf::Vector2f parent_pos,
        sf::Vector2f parent_size) override;

    void render(const Ui::TexturedButton& button, const float dt, sf::Vector2f parent_pos,
        sf::Vector2f parent_size);
};

