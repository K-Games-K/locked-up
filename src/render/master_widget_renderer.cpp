#include "render/master_widget_renderer.hpp"

MasterWidgetRenderer::MasterWidgetRenderer(sf::RenderWindow& window)
    : text_renderer(window),
    button_renderer(window),
    panel_renderer(window),
    line_edit_renderer(window),
    checkbox_renderer(window),
    table_widget_renderer(window) {}

void MasterWidgetRenderer::render(const Ui::Widget& widget, const float dt, sf::Vector2f parent_pos,
    sf::Vector2f parent_size)
{
    if(auto text = dynamic_cast<const Ui::Text*>(&widget))
    {
        text_renderer.render(*text, dt, parent_pos, parent_size);
    }
    else if(auto button = dynamic_cast<const Ui::Button*>(&widget))
    {
        button_renderer.render(*button, dt, parent_pos, parent_size);
    }
    else if(auto panel = dynamic_cast<const Ui::Panel*>(&widget))
    {
        panel_renderer.render(*panel, dt, parent_pos, parent_size);
    }
    else if(auto line_edit = dynamic_cast<const Ui::LineEdit*>(&widget))
    {
        line_edit_renderer.render(*line_edit, dt, parent_pos, parent_size);
    }
    else if(auto checkbox = dynamic_cast<const Ui::Checkbox*>(&widget))
    {
        checkbox_renderer.render(*checkbox, dt, parent_pos, parent_size);
    }
    else if(auto table_widget = dynamic_cast<const Ui::TableWidget*>(&widget))
    {
        table_widget_renderer.render(*table_widget, dt, parent_pos, parent_size);
    }

    sf::Vector2f widget_pos = widget.get_global_position(parent_pos, parent_size);
    sf::Vector2f widget_size = widget.get_global_size(parent_size);
    for(const auto& child : widget.get_children())
        if(child != nullptr && child->is_enabled())
            render(*child, dt, widget_pos, widget_size);
}
