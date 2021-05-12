#pragma once

#include "button.hpp"
#include "panel.hpp"

namespace Ui
{
    class BigPopup : public Ui::Panel
    {
    public:
        using CloseCallback = std::function<void(BigPopup&)>;

        BigPopup(const sf::Texture& background_texture, const sf::Font& font);

        BigPopup(const BigPopup& other);

        void show(const std::string& title, const std::string& description);

        BigPopup& set_close_callback(CloseCallback close_callback);

    private:
        Text* title_text;
        Text* description_text;
        Button* close_button;

        CloseCallback close_callback;

        void close_clicked();

        BigPopup* clone() const override;
    };
}
