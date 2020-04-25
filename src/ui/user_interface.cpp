#include "ui/user_interface.hpp"

namespace Ui
{
    UserInterface::UserInterface(sf::Vector2f position, sf::Vector2f size)
        : Widget(WidgetType::UserInterface, *this, position, size)
    {}

    void UserInterface::update()
    {

    }

    void UserInterface::add_widget(std::unique_ptr<Widget>&& widget)
    {
        widgets.push_back(std::move(widget));
    }

    const std::vector<std::unique_ptr<Widget>>& UserInterface::get_widgets() const
    {
        return widgets;
    }

    sf::Vector2f UserInterface::get_absolute_position() const
    {
        // UserInterface is top widget in hierarchy. It has no parent.
        return get_position();
    }
}
