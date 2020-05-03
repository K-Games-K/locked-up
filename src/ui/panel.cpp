#include "ui/panel.hpp"

namespace Ui
{
    Panel& Panel::set_background_color(Color background_color)
    {
        this->background_color = background_color;

        return *this;
    }

    Color Panel::get_background_color() const
    {
        return background_color;
    }

    Widget* Panel::clone() const
    {
        return new Panel(*this);
    }
}
