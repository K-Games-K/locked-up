#include "ui/textured_button.hpp"

namespace Ui
{
    TexturedButton::TexturedButton(const sf::Texture& default_texture,
        const sf::Texture& hover_texture, const sf::Texture& active_texture)
        : default_texture(&default_texture), hover_texture(&hover_texture),
        active_texture(&active_texture)
    {
        set_size((sf::Vector2f) default_texture.getSize());
    }

    const sf::Texture& TexturedButton::get_texture() const
    {
        if(activated)
            return *active_texture;
        else if(hovered)
            return *hover_texture;
        else
            return *default_texture;
    }

    TexturedButton& TexturedButton::set_default_texture(const sf::Texture& default_texture)
    {
        this->default_texture = &default_texture;

        return *this;
    }

    TexturedButton& TexturedButton::set_hover_texture(const sf::Texture& hover_texture)
    {
        this->hover_texture = &hover_texture;

        return *this;
    }

    TexturedButton& TexturedButton::set_active_texture(const sf::Texture& active_texture)
    {
        this->active_texture = &active_texture;

        return *this;
    }

    TexturedButton* TexturedButton::clone() const
    {
        return new TexturedButton(*this);
    }
}
