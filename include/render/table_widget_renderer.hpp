#pragma once

#include "widget_container_renderer.hpp"
#include "ui/table_widget.hpp"

class TableWidgetRenderer : public WidgetContainerRenderer<Ui::TableWidget>
{
public:
    TableWidgetRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::TableWidget& table_widget, const float dt) override;
};
