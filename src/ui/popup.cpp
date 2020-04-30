#include "ui/popup.hpp"

namespace Ui
{

    Popup::Popup(const sf::Texture& texture, const sf::Font& font, sf::Vector2f position,
        Anchor origin, Anchor anchor)
        : TexturedPanel(texture, position, origin, anchor)
    {
        set_enabled(false);

        title_text = new Ui::Text(
            "",
            font,
            {0, 50},
            {sf::Color::Black, 32},
            Anchor::Center, Anchor::CenterTop
        );
        add_widget(title_text);

        description_text = new Ui::Text(
            "",
            font,
            {0, 10},
            {sf::Color::Black},
            Anchor::Center, Anchor::Center
        );
        add_widget(description_text);

        add_widget(
            new Ui::Button(
                "X",
                font,
                std::bind(&Popup::close_clicked, this, std::placeholders::_1),
                {-8, 8}, {30, 30},
                {
                    sf::Color::Transparent,
                    sf::Color(0, 0, 0, 100),
                    sf::Color(0, 0, 0, 200)
                },
                {sf::Color::Black, 30},
                Ui::Anchor::TopRight, Ui::Anchor::TopRight
            )
        );
    }

    void Popup::show()
    {
        set_enabled(true);
    }

    void Popup::set_title(const std::string& title)
    {
        title_text->set_string(title);
    }

    void Popup::set_description(const std::string& description)
    {
        description_text->set_string(description);
    }

    void Popup::close_clicked(Button& button)
    {
        set_enabled(false);
    }
}
