#include "ui/big_popup.hpp"

namespace Ui
{
    BigPopup::BigPopup(const sf::Texture& background_texture, const sf::Font& font)
        : Panel(background_texture)
    {
        set_enabled(false);

        title_text = (Ui::Text*) add_widget(
            Ui::Text(font)
                .set_font_size(32)
                .set_max_width(get_size().x)
                .set_position({0, 50})
                .set_anchor(Anchor::CenterTop)
        );

        description_text = (Ui::Text*) add_widget(
            Ui::Text(font)
                .set_max_width(get_size().x - 20)
                .set_position({0, 10})
        );

        close_button = (Ui::Button*) add_widget(
            Ui::Button()
                .set_callback(std::bind(&BigPopup::close_clicked, this))
                .set_hover_color(Color(0, 0, 0, 100))
                .set_active_color(Color(0, 0, 0, 200))
                .set_position({-10, 10})
                .set_size({30, 30})
                .set_origin(Origin::TopRight)
                .set_anchor(Anchor::TopRight)
        );
        close_button->add_widget(
            Text(font, "X")
                .set_color(Color::Black)
                .set_font_size(30)
                .set_origin(Origin::Center)
                .set_anchor(Anchor::Center)
        );
    }

    BigPopup::BigPopup(const BigPopup& other)
        : Panel(other)
    {
        title_text = get_child<Ui::Text>(0);
        description_text = get_child<Ui::Text>(1);
        close_button = get_child<Ui::Button>(2);
        close_button->set_callback(std::bind(&BigPopup::close_clicked, this));
    }

    void BigPopup::show(const std::string& title, const std::string& description)
    {
        title_text->set_string(title);
        description_text->set_string(description);
        set_enabled(true);
    }

    BigPopup& BigPopup::set_close_callback(CloseCallback close_callback)
    {
        this->close_callback = close_callback;

        return *this;
    }

    void BigPopup::close_clicked()
    {
        if(is_enabled())
        {
            if(close_callback)
                close_callback(*this);

            set_enabled(false);
        }
    }

    BigPopup* BigPopup::clone() const
    {
        return new BigPopup(*this);
    }
}
