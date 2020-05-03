#include "ui/textured_panel.hpp"

namespace Ui
{
    TexturedPanel::TexturedPanel(const sf::Texture& background_texture)
        : background_texture(&background_texture)
    {
        set_size((sf::Vector2f) background_texture.getSize());
    }

    TexturedPanel& TexturedPanel::set_background_texture(const sf::Texture& background_texture)
    {
        this->background_texture = &background_texture;

        return *this;
    }

    const sf::Texture& TexturedPanel::get_background_texture() const
    {
        return *background_texture;
    }

    TexturedPanel* TexturedPanel::clone() const
    {
        return new TexturedPanel(*this);
    }
}
