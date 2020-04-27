#pragma once

#include "panel.hpp"

namespace Ui
{
    class TexturedPanel : public Panel
    {
    private:
        const sf::Texture* texture;

    public:
        TexturedPanel(const sf::Texture& texture, sf::Vector2f position = {0, 0},
            Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

        void set_texture(const sf::Texture& texture);

        const sf::Texture& get_texture() const;
    };
}
