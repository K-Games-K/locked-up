#pragma once

#include "widget_renderer.hpp"
#include "ui/checkbox.hpp"
#include "ui/textured_checkbox.hpp"

class CheckboxRenderer : public WidgetRenderer<Ui::Checkbox>
{
public:
    CheckboxRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::Checkbox& checkbox, const float dt) override;

    void render(const Ui::TexturedCheckbox& checkbox, const float dt);
};
