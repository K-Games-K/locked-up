#include "ui/textured_button.hpp"

namespace Ui
{
    TexturedButton::TexturedButton(const Ui::Widget& parent, const std::string& text,
        const sf::Font& font, const ButtonTextures& textures, Ui::Button::Callback callback,
        sf::Vector2f position, Ui::Anchor origin, Ui::Anchor anchor)
        : Button(
        WidgetType::TexturedButton, parent, text, font, callback, position,
        (sf::Vector2f) textures.default_texture.getSize(), ButtonColors(), origin, anchor
    )
    {
        set_textures(textures);
    }

    void TexturedButton::set_textures(const ButtonTextures& textures)
    {
        default_texture = &textures.default_texture;
        hover_texture = &textures.hover_texture;
        active_texture = &textures.active_texture;
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
}
