#pragma once

#include "widget_renderer.hpp"
#include "ui/checkbox.hpp"
#include "ui/textured_checkbox.hpp"

class CheckboxRender : public WidgetRenderer<Ui::Checkbox>
{
private:
    sf::Sprite checkbox_sprite;

public:
    CheckboxRender(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::Checkbox& checkbox, const float dt) override;

    void render(const Ui::TexturedCheckbox& checkbox, const float dt);
};
