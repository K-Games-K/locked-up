#pragma once

#include <SFML/Graphics.hpp>

#include "widget.hpp"

namespace Ui
{
    class Panel : public Widget
    {
    public:
        Panel& set_background_color(Color background_color);

        Color get_background_color() const;

    private:
        Color background_color = Color::Transparent;

        Widget* clone() const override;
    };
}
