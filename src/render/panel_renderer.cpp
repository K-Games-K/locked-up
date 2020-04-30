#include "render/panel_renderer.hpp"

PanelRenderer::PanelRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : WidgetContainerRenderer(window, resources),
    table_widget_renderer(window, resources)
{}

void PanelRenderer::render(const Ui::Panel& panel, const float dt)
{
    if(!panel.is_enabled())
        return;

    sf::Vector2f panel_position = panel.get_relative_position(origin_pos, parent_size);
    sf::RectangleShape panel_rect(panel.get_size());
    panel_rect.setPosition(panel_position);
    panel_rect.setFillColor(panel.get_background_color());
    window.draw(panel_rect);

    for(const auto& widget : panel.get_widgets())
    {
        if(auto textured_panel = dynamic_cast<Ui::TexturedPanel*>(widget))
        {
            sf::Vector2f temp_pos = origin_pos;
            sf::Vector2f temp_size = parent_size;
            set_origin_pos(panel_position);
            set_parent_size(panel.get_size());
            render(*textured_panel, dt);
            origin_pos = temp_pos;
            parent_size = temp_size;
        }
        else if(auto sub_panel = dynamic_cast<Ui::Panel*>(widget))
        {
            sf::Vector2f temp_pos = origin_pos;
            sf::Vector2f temp_size = parent_size;
            set_origin_pos(panel_position);
            set_parent_size(panel.get_size());
            render(*sub_panel, dt);
            origin_pos = temp_pos;
            parent_size = temp_size;
        }
        else if(auto table_widget = dynamic_cast<Ui::TableWidget*>(widget))
        {
            table_widget_renderer.set_origin_pos(panel_position);
            table_widget_renderer.set_parent_size(panel.get_size());
            table_widget_renderer.render(*table_widget, dt);
        }
        else
        {
            WidgetContainerRenderer::render(widget, dt, panel_position, panel.get_size());
        }
    }
}

void PanelRenderer::render(const Ui::TexturedPanel& panel, const float dt)
{
    if(!panel.is_enabled())
        return;

    sf::Vector2f panel_position = panel.get_relative_position(origin_pos, parent_size);
    sf::Sprite panel_sprite(panel.get_texture());
    panel_sprite.setPosition(panel_position);
    window.draw(panel_sprite);

    render(dynamic_cast<const Ui::Panel&>(panel), dt);
}
