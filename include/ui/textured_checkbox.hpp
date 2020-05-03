#pragma once

#include "checkbox.hpp"

namespace Ui
{
    class TexturedCheckbox : public Checkbox
    {
    public:
        TexturedCheckbox(const sf::Texture& unchecked_texture, const sf::Texture& checked_texture,
            bool checked = false);

        TexturedCheckbox& set_unchecked_texture(const sf::Texture& unchecked_texture);

        TexturedCheckbox& set_checked_texture(const sf::Texture& checked_texture);

        const sf::Texture& get_texture() const;

    private:
        const sf::Texture* unchecked_texture;
        const sf::Texture* checked_texture;

        TexturedCheckbox* clone() const override;
    };
}
