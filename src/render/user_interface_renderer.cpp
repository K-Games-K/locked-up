#include "render/user_interface_renderer.hpp"

UserInterfaceRenderer::UserInterfaceRenderer(sf::RenderWindow& window, ResourceManagers resources)
    : Renderer(window, resources),
    text_renderer(window, resources),
    button_renderer(window, resources)
{}

void UserInterfaceRenderer::render(const Ui::UserInterface& ui, sf::Vector2f origin)
{
    for(const auto& widget : ui.get_widgets())
    {
        switch(widget->get_type())
        {
            case Ui::WidgetType::Text:
                text_renderer.render(dynamic_cast<Ui::Text&>(*widget));
                break;
            case Ui::WidgetType::Button:
                button_renderer.render(dynamic_cast<Ui::Button&>(*widget));
                break;
            default:
                break;
        }
    }
}
