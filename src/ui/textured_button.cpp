#include "ui/textured_button.hpp"

namespace Ui
{
    TexturedButton::TexturedButton(const Ui::Widget& parent, const std::string& text,
        const sf::Font& font, const sf::Texture& texture, Ui::Button::Callback callback,
        sf::Vector2f position, Ui::Anchor origin, Ui::Anchor anchor)
        : Button(
        WidgetType::TexturedButton, parent, text, font, callback, position,
        (sf::Vector2f) texture.getSize(), ButtonColors(), origin, anchor
    ), texture(&texture)
    {}

    void TexturedButton::set_texture(const sf::Texture& texture)
    {
        this->texture = &texture;
    }

    const sf::Texture& TexturedButton::get_texture() const
    {
        return *texture;
    }
}
