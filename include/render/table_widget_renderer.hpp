#pragma once

#include "widget_renderer.hpp"
#include "ui/table_widget.hpp"

class TableWidgetRenderer : public WidgetRenderer<Ui::TableWidget>
{
public:
    TableWidgetRenderer(sf::RenderWindow& window);

    void render(const Ui::TableWidget& table_widget, const float dt, sf::Vector2f parent_pos,
        sf::Vector2f parent_size) override;
};
