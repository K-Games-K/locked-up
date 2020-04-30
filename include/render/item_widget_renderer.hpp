#pragma once

#include "widget_renderer.hpp"
#include "ui/item_widget.hpp"
#include "items/item.hpp"

class ItemWidgetRenderer : public WidgetRenderer<std::vector<Ui::ItemWidget>>
{
private:

public:
    ItemWidgetRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const std::vector<Ui::ItemWidget>& items , const float dt) override;

};

