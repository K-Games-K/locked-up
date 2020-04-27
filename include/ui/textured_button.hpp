#pragma once

#include "button.hpp"

namespace Ui
{
    struct ButtonTextures
    {
        const sf::Texture& default_texture;
        const sf::Texture& hover_texture;
        const sf::Texture& active_texture;
    };

    class TexturedButton : public Button
    {
    private:
        ButtonTextures textures;

    public:
        TexturedButton(const std::string& text, const sf::Font& font,
            ButtonTextures textures, Callback callback, sf::Vector2f position = {0, 0},
            Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

        const sf::Texture& get_texture() const;
    };
}
