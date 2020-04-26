#include "render/panel_renderer.hpp"

PanelRenderer::PanelRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : Renderer(window, resources),
    text_renderer(window, resources),
    button_renderer(window, resources),
    textured_button_renderer(window, resources),
    text_edit_renderer(window, resources)
{}

void PanelRenderer::render(const Ui::Panel& ui, const float dt)
{
    text_renderer.set_origin_pos(ui.get_local_position());
    text_renderer.set_parent_size(ui.get_size());
    button_renderer.set_origin_pos(ui.get_local_position());
    button_renderer.set_parent_size(ui.get_size());
    textured_button_renderer.set_origin_pos(ui.get_local_position());
    textured_button_renderer.set_parent_size(ui.get_size());
    text_edit_renderer.set_origin_pos(ui.get_local_position());
    text_edit_renderer.set_parent_size(ui.get_size());

    for(const auto& widget : ui.get_widgets())
    {
        switch(widget->get_type())
        {
            case Ui::WidgetType::Text:
                text_renderer.render(dynamic_cast<Ui::Text&>(*widget), dt);
                break;
            case Ui::WidgetType::Button:
                button_renderer.render(dynamic_cast<Ui::Button&>(*widget), dt);
                break;
            case Ui::WidgetType::TexturedButton:
                textured_button_renderer.render(dynamic_cast<Ui::TexturedButton&>(*widget), dt);
                break;
            case Ui::WidgetType::TextEdit:
                text_edit_renderer.render(dynamic_cast<Ui::TextEdit&>(*widget), dt);
                break;
            default:
                break;
        }
    }
}
