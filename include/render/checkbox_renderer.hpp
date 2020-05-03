#pragma once

#include "widget_renderer.hpp"
#include "ui/checkbox.hpp"
#include "ui/textured_checkbox.hpp"

class CheckboxRenderer : public WidgetRenderer<Ui::Checkbox>
{
public:
    CheckboxRenderer(sf::RenderWindow& window);

    void render(const Ui::Checkbox& checkbox, const float dt, sf::Vector2f parent_pos, sf::Vector2f parent_size) override;

    void render(const Ui::TexturedCheckbox& checkbox, const float dt, sf::Vector2f parent_pos, sf::Vector2f parent_size);
};
