#pragma once

#include "ui/button.hpp"
#include "ui/textured_panel.hpp"

namespace Ui
{
    class Popup : public Ui::TexturedPanel
    {
    private:
        Text* title_text;
        Text* description_text;

        void close_clicked(Button& button);

    public:
        Popup(const sf::Texture& texture, const sf::Font& font, sf::Vector2f position = {0, 0},
            Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

        void show();

        void set_title(const std::string& title);

        void set_description(const std::string& description);
    };
}
