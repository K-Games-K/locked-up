#pragma once

#include <SFML/Graphics.hpp>

#include "widget.hpp"

namespace Ui
{
    class Panel : public Widget
    {
    public:
        Panel() = default;

        Panel(const sf::Texture& background_texture);

        Panel& set_background_color(Color background_color);

        Color get_background_color() const;

        Panel& set_background_texture(const sf::Texture& background_texture);

        const sf::Texture& get_background_texture() const;

        bool has_background_texture() const;

    private:
        Color background_color = Color::Transparent;
        const sf::Texture* background_texture = nullptr;

        Widget* clone() const override;
    };
}
