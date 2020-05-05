#pragma once

#include "ui/button.hpp"
#include "ui/textured_panel.hpp"

namespace Ui
{
    class Popup : public Ui::TexturedPanel
    {
    public:
        using CloseCallback = std::function<void(Popup&)>;

        Popup(const sf::Texture& background_texture, const sf::Font& font);

        Popup(const Popup& other);

        void show(const std::string& title, const std::string& description);

        Popup& set_close_callback(CloseCallback close_callback);

    private:
        Text* title_text;
        Text* description_text;
        Button* close_button;

        CloseCallback close_callback;

        void close_clicked();

        Popup* clone() const override;
    };
}
