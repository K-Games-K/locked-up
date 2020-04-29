#pragma once

#include "widget_renderer.hpp"
#include "ui/visual_item.hpp"
#include "items/item.hpp"

class VisualItemRenderer : public WidgetRenderer<Ui::VisualItem>
{
private:

public:
    VisualItemRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::VisualItem& item , const float dt) override;

};

