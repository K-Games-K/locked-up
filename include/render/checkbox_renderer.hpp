#pragma once

#include "widget_renderer.hpp"
#include "ui/checkbox.hpp"

class CheckboxRender : public WidgetRenderer<Ui::Checkbox>
{
public:
    CheckboxRender(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::Checkbox& checkbox, const float dt) override;
};
