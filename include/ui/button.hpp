#pragma once

#include <functional>

#include "widget.hpp"
#include "text.hpp"

namespace Ui
{
    struct ButtonColors
    {
        sf::Color default_color = sf::Color::White;
        sf::Color hover_color = sf::Color::White;
        sf::Color active_color = sf::Color::White;
    };

    class Button : public Widget
    {
    private:
        using Callback = std::function<void(Button&)>;

        Text text;
        ButtonColors colors;
        Callback callback;

        bool activated = false;
        bool hovered = false;

    public:
        Button(const Widget& parent, const std::string& text, const sf::Font& font,
            Callback callback, sf::Vector2f position = {0, 0}, sf::Vector2f size = {0, 0},
            ButtonColors colors = ButtonColors(), Anchor origin = Anchor::TopLeft,
            Anchor anchor = Anchor::TopLeft);

        void set_activated(bool activated);

        void set_hovered(bool hovered);

        void set_text(Text text);

        const Text& get_text() const;

        void  set_colors(ButtonColors colors);

        ButtonColors get_colors() const;

        sf::Color get_color() const;

        void set_callback(Callback callback);
    };
}
