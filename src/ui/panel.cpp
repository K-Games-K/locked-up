#include <algorithm>

#include "ui/panel.hpp"
#include "ui/button.hpp"

namespace Ui
{
    Panel::Panel(sf::Vector2f position, sf::Vector2f size, sf::Color background_color,
        Anchor origin, Anchor anchor)
        : Widget(position, size, origin, anchor),
        background_color(background_color)
    {}

    Panel::~Panel()
    {
        for(auto& widget : widgets)
            delete widget;
    }

    void Panel::handle_event(sf::Event event, sf::Vector2f mouse_pos)
    {
        if(!is_enabled())
            return;

        for(auto& widget : widgets)
        {
            if(!widget->is_enabled())
                continue;

            widget->handle_event(
                event, mouse_pos - widget->get_relative_position(
                    get_local_position(),
                    get_size()
                ));
        }
    }

    void Panel::add_widget(Widget* widget)
    {
        widgets.push_back(widget);
    }

    void Panel::remove_widget(Widget* widget)
    {
        widgets.erase(
            std::find(widgets.begin(), widgets.end(), widget)
        );
    }

    const std::vector<Widget*>& Panel::get_widgets() const
    {
        return widgets;
    }

    void Panel::set_background_color(sf::Color background_color)
    {
        this->background_color = background_color;
    }

    sf::Color Panel::get_background_color() const
    {
        return background_color;
    }

    void Panel::reset()
    {
        for(auto& widget : widgets)
            widget->reset();
    }
}
