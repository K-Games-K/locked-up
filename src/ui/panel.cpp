#include "panel.hpp"

namespace Ui
{
    Panel::Panel(const sf::Texture& background_texture)
        : background_texture(&background_texture)
    {
        set_size((sf::Vector2f) background_texture.getSize());
    }

    Panel& Panel::set_background_color(Color background_color)
    {
        this->background_color = background_color;

        return *this;
    }

    Color Panel::get_background_color() const
    {
        return background_color;
    }

    Panel& Panel::set_background_texture(const sf::Texture& background_texture)
    {
        this->background_texture = &background_texture;
        set_size((sf::Vector2f) background_texture.getSize());

        return *this;
    }

    const sf::Texture& Panel::get_background_texture() const
    {
        return *background_texture;
    }

    bool Panel::has_background_texture() const
    {
        return background_texture != nullptr;
    }

    Widget* Panel::clone() const
    {
        return new Panel(*this);
    }
}
