#pragma once

#include "button.hpp"

namespace Ui
{
    class TexturedButton : public Button
    {
    public:
        TexturedButton(const sf::Texture& default_texture, const sf::Texture& hover_texture,
            const sf::Texture& active_texture);

        TexturedButton& set_default_texture(const sf::Texture& default_texture);

        TexturedButton& set_hover_texture(const sf::Texture& hover_texture);

        TexturedButton& set_active_texture(const sf::Texture& active_texture);

        const sf::Texture& get_texture() const;

    private:
        const sf::Texture* default_texture;
        const sf::Texture* hover_texture;
        const sf::Texture* active_texture;

        TexturedButton* clone() const override;
    };
}
