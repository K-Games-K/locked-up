#include "render/panel_renderer.hpp"

PanelRenderer::PanelRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : WidgetRenderer(window, resources),
    text_renderer(window, resources),
    button_renderer(window, resources),
    text_edit_renderer(window, resources),
    checkbox_render(window, resources)
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
        switch(widget->get_type())
        {
            case Ui::WidgetType::Text:
                text_renderer.set_origin_pos(panel_position);
                text_renderer.set_parent_size(panel.get_size());
                text_renderer.render(dynamic_cast<Ui::Text&>(*widget), dt);
                break;
            case Ui::WidgetType::Button:
                button_renderer.set_origin_pos(panel_position);
                button_renderer.set_parent_size(panel.get_size());
                button_renderer.render(dynamic_cast<Ui::Button&>(*widget), dt);
                break;
            case Ui::WidgetType::TexturedButton:
                button_renderer.set_origin_pos(panel_position);
                button_renderer.set_parent_size(panel.get_size());
                button_renderer.render(dynamic_cast<Ui::TexturedButton&>(*widget), dt);
                break;
            case Ui::WidgetType::TextEdit:
                text_edit_renderer.set_origin_pos(panel_position);
                text_edit_renderer.set_parent_size(panel.get_size());
                text_edit_renderer.render(dynamic_cast<Ui::TextEdit&>(*widget), dt);
                break;
            case Ui::WidgetType::Panel:
            {
                sf::Vector2f temp_pos = origin_pos;
                sf::Vector2f temp_size = parent_size;
                set_origin_pos(panel_position);
                set_parent_size(panel.get_size());
                PanelRenderer::render(dynamic_cast<Ui::Panel&>(*widget), dt);
                origin_pos = temp_pos;
                parent_size = temp_size;
                break;
            }
            case Ui::WidgetType::TexturedPanel:
            {
                sf::Vector2f temp_pos = origin_pos;
                sf::Vector2f temp_size = parent_size;
                set_origin_pos(panel_position);
                set_parent_size(panel.get_size());
                render(dynamic_cast<Ui::TexturedPanel&>(*widget), dt);
                origin_pos = temp_pos;
                parent_size = temp_size;
                break;
            }
            case Ui::WidgetType::Checkbox:
            {
                checkbox_render.set_origin_pos(panel_position);
                checkbox_render.set_parent_size(panel.get_size());
                checkbox_render.render(dynamic_cast<Ui::Checkbox&>(*widget), dt);
                break;
            }
            case Ui::WidgetType::TexturedCheckbox:
            {
                checkbox_render.set_origin_pos(panel_position);
                checkbox_render.set_parent_size(panel.get_size());
                checkbox_render.render(dynamic_cast<Ui::TexturedCheckbox&>(*widget), dt);
                break;
            }
            default:
                break;
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
