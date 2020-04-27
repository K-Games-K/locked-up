#include "ui/textured_panel.hpp"

namespace Ui
{
    TexturedPanel::TexturedPanel(const sf::Texture& texture, sf::Vector2f position, Anchor origin,
        Anchor anchor)
        : Panel(
        WidgetType::TexturedPanel, position, (sf::Vector2f) texture.getSize(),
        sf::Color::Transparent, origin, anchor
    ), texture(&texture)
    {
    }

    void TexturedPanel::set_texture(const sf::Texture& texture)
    {
        this->texture = &texture;
    }

    const sf::Texture& TexturedPanel::get_texture() const
    {
        return *texture;
    }
}
