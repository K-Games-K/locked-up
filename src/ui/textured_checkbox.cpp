#include "ui/textured_checkbox.hpp"

namespace Ui
{
    TexturedCheckbox::TexturedCheckbox(const sf::Texture& unchecked_texture,
        const sf::Texture& checked_texture, bool checked)
        : Checkbox(checked), unchecked_texture(&unchecked_texture),
        checked_texture(&checked_texture)
    {
        set_size((sf::Vector2f) unchecked_texture.getSize());
    }

    TexturedCheckbox& TexturedCheckbox::set_unchecked_texture(const sf::Texture& unchecked_texture)
    {
        this->unchecked_texture = &unchecked_texture;

        return *this;
    }

    TexturedCheckbox& TexturedCheckbox::set_checked_texture(const sf::Texture& checked_texture)
    {
        this->checked_texture = &checked_texture;

        return *this;
    }

    const sf::Texture& TexturedCheckbox::get_texture() const
    {
        if(checked)
            return *checked_texture;
        else
            return *unchecked_texture;
    }

    TexturedCheckbox* TexturedCheckbox::clone() const
    {
        return new TexturedCheckbox(*this);
    }
}

