#include <algorithm>

#include "ui/user_interface.hpp"
#include "ui/button.hpp"

namespace Ui
{
    UserInterface::UserInterface(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size)
        : Widget(WidgetType::UserInterface, position, size), window(window)
    {}

    UserInterface::~UserInterface()
    {
        for(auto& widget : widgets)
            delete widget;
    }

    void UserInterface::handle_event(sf::Event event)
    {
        for(auto& widget: widgets)
        {
            switch(widget->get_type())
            {
                case Ui::WidgetType::Button:
                case Ui::WidgetType::TexturedButton:
                {
                    auto button = dynamic_cast<Ui::Button*>(widget);
                    sf::Vector2f mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(window);
                    sf::FloatRect widget_box(
                        widget->get_relative_position(
                            get_local_position(),
                            get_size()
                        ), widget->get_size());

                    if(event.type == sf::Event::MouseButtonPressed &&
                        widget_box.contains(mouse_pos))
                        button->set_activated(true);
                    if(event.type == sf::Event::MouseButtonReleased)
                        button->set_activated(false);
                    if(event.type == sf::Event::MouseMoved)
                        button->set_hovered(widget_box.contains(mouse_pos));
                    break;
                }
                default:
                    break;
            }
        }
    }

    void UserInterface::add_widget(Widget* widget)
    {
        widgets.push_back(widget);
    }

    void UserInterface::remove_widget(Widget* widget)
    {
        widgets.erase(
            std::find(widgets.begin(), widgets.end(), widget)
        );
    }

    const std::vector<Widget*>& UserInterface::get_widgets() const
    {
        return widgets;
    }
}
