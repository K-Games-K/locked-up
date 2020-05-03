#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "ui/notification_widget.hpp"

class NotificationWidgetRenderer : public WidgetRenderer<Ui::NotificationWidget>
{
private:
    TextRenderer text_renderer;

public:
    NotificationWidgetRenderer(sf::RenderWindow& window);

    void render(const Ui::NotificationWidget& notification_widget, const float dt,
        sf::Vector2f parent_pos, sf::Vector2f parent_size);
};
