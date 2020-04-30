#pragma once

#include "widget_container_renderer.hpp"
#include "table_widget_renderer.hpp"
#include "ui/panel.hpp"
#include "ui/textured_panel.hpp"

class PanelRenderer : public WidgetContainerRenderer<Ui::Panel>
{
private:
    TableWidgetRenderer table_widget_renderer;

public:
    PanelRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::Panel& panel, const float dt) override;

    void render(const Ui::TexturedPanel& panel, const float dt);
};
