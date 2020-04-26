#pragma once

#include "button.hpp"

namespace Ui
{
    class TexturedButton : public Button
    {
    private:
        const sf::Texture* texture;

    public:
        TexturedButton(const Widget& parent, const std::string& text, const sf::Font& font,
            const sf::Texture& texture, Callback callback, sf::Vector2f position = {0, 0},
            Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

        void set_texture(const sf::Texture& texture);

        const sf::Texture& get_texture() const;
    };
}
