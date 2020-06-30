#pragma once

#include "widget_renderer.hpp"
#include "table_widget_renderer.hpp"
#include "../ui/panel.hpp"

class PanelRenderer : public WidgetRenderer<Ui::Panel>
{
public:
    PanelRenderer(sf::RenderWindow& window);

    void render(const Ui::Panel& panel, const float dt, sf::Vector2f parent_pos,
        sf::Vector2f parent_size) override;
};
