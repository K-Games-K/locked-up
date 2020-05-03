#pragma once

#include "panel.hpp"

namespace Ui
{
    class TexturedPanel : public Panel
    {
    public:
        TexturedPanel(const sf::Texture& background_texture);

        TexturedPanel& set_background_texture(const sf::Texture& background_texture);

        const sf::Texture& get_background_texture() const;

    private:
        const sf::Texture* background_texture;

        TexturedPanel* clone() const override;
    };
}
