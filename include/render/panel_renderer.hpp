#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "button_renderer.hpp"
#include "text_edit_renderer.hpp"
#include "checkbox_renderer.hpp"
#include "ui/panel.hpp"
#include "ui/textured_panel.hpp"

class PanelRenderer : public WidgetRenderer<Ui::Panel>
{
private:
    TextRenderer text_renderer;
    ButtonRenderer button_renderer;
    TextEditRenderer text_edit_renderer;
    CheckboxRender checkbox_render;

public:
    PanelRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::Panel& panel, const float dt) override;

    void render(const Ui::TexturedPanel& panel, const float dt);
};
