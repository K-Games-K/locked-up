#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "button_renderer.hpp"
#include "line_edit_renderer.hpp"
#include "panel_renderer.hpp"
#include "checkbox_renderer.hpp"
#include "notification_widget_renderer.hpp"
#include "ui/widget.hpp"

class MasterWidgetRenderer
{
private:
    TextRenderer text_renderer;
    ButtonRenderer button_renderer;
    CheckboxRenderer checkbox_renderer;
    PanelRenderer panel_renderer;
    LineEditRenderer line_edit_renderer;
    NotificationWidgetRenderer notification_widget_renderer;
    TableWidgetRenderer table_widget_renderer;

public:
    MasterWidgetRenderer(sf::RenderWindow& window);

    void render(const Ui::Widget& widget, const float dt, sf::Vector2f parent_pos,
        sf::Vector2f parent_size);
};
