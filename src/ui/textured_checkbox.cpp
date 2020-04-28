#include "ui/textured_checkbox.hpp"

namespace Ui
{
    TexturedCheckbox::TexturedCheckbox(CheckboxTextures textures, sf::Vector2f position,
        Anchor origin, Anchor anchor)
        : Checkbox(
        WidgetType::TexturedCheckbox, position, (sf::Vector2f) textures.unchecked_texture.getSize(),
        {}, origin, anchor
    ), textures(textures)
    {}

    const sf::Texture& TexturedCheckbox::get_texture() const
    {
        if(checked)
            return textures.checked_texture;
        else
            return textures.unchecked_texture;
    }
}

