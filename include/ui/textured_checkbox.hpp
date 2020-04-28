#pragma once

#include "checkbox.hpp"

namespace Ui
{
    struct CheckboxTextures
    {
        const sf::Texture& unchecked_texture;
        const sf::Texture& checked_texture;
    };

    class TexturedCheckbox : public Checkbox
    {
    private:
        CheckboxTextures textures;

    public:
        TexturedCheckbox(CheckboxTextures textures, sf::Vector2f position = {0, 0},
            Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

        const sf::Texture& get_texture() const;
    };
}
