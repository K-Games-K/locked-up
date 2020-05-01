#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "button_renderer.hpp"
#include "text_edit_renderer.hpp"
#include "checkbox_renderer.hpp"
#include "notification_widget_renderer.hpp"

template<typename T>
class WidgetContainerRenderer : public WidgetRenderer<T>
{
private:
    TextRenderer text_renderer;
    ButtonRenderer button_renderer;
    TextEditRenderer text_edit_renderer;
    CheckboxRenderer checkbox_renderer;
    NotificationWidgetRenderer notification_widget_renderer;

protected:
    WidgetContainerRenderer(sf::RenderWindow& window, ResourceManagers resources)
        : WidgetRenderer<T>(window, resources),
        text_renderer(window, resources),
        button_renderer(window, resources),
        text_edit_renderer(window, resources),
        checkbox_renderer(window, resources),
        notification_widget_renderer(window, resources)
    {}

    void render(const Ui::Widget* widget, const float dt, sf::Vector2f origin_pos,
        sf::Vector2f parent_size)
    {
        if(widget == nullptr || !widget->is_enabled())
            return;

        if(auto text = dynamic_cast<const Ui::Text*>(widget))
        {
            text_renderer.set_origin_pos(origin_pos);
            text_renderer.set_parent_size(parent_size);
            text_renderer.render(*text, dt);
        }
        else if(auto textured_button = dynamic_cast<const Ui::TexturedButton*>(widget))
        {
            button_renderer.set_origin_pos(origin_pos);
            button_renderer.set_parent_size(parent_size);
            button_renderer.render(*textured_button, dt);
        }
        else if(auto button = dynamic_cast<const Ui::Button*>(widget))
        {
            button_renderer.set_origin_pos(origin_pos);
            button_renderer.set_parent_size(parent_size);
            button_renderer.render(*button, dt);
        }
        else if(auto text_edit = dynamic_cast<const Ui::TextEdit*>(widget))
        {
            text_edit_renderer.set_origin_pos(origin_pos);
            text_edit_renderer.set_parent_size(parent_size);
            text_edit_renderer.render(*text_edit, dt);
        }
        else if(auto textured_checkbox = dynamic_cast<const Ui::TexturedCheckbox*>(widget))
        {
            checkbox_renderer.set_origin_pos(origin_pos);
            checkbox_renderer.set_parent_size(parent_size);
            checkbox_renderer.render(*textured_checkbox, dt);
        }
        else if(auto checkbox = dynamic_cast<const Ui::Checkbox*>(widget))
        {
            checkbox_renderer.set_origin_pos(origin_pos);
            checkbox_renderer.set_parent_size(parent_size);
            checkbox_renderer.render(*checkbox, dt);
        }
        else if(auto notification_widget = dynamic_cast<const Ui::NotificationWidget*>(widget))
        {
            notification_widget_renderer.set_origin_pos(origin_pos);
            notification_widget_renderer.set_parent_size(parent_size);
            notification_widget_renderer.render(*notification_widget, dt);
        }
    }
};
