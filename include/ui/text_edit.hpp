#pragma once

#include "widget.hpp"
#include "text.hpp"

namespace Ui
{
    struct TextEditColors
    {
        sf::Color background_color = sf::Color::Black;
        sf::Color border_color = sf::Color::White;
        sf::Color text_color = sf::Color::White;
    };

    class TextEdit : public Widget
    {
    protected:
        bool activated = false;

        TextEdit(WidgetType type, const sf::Font& font, sf::Vector2f position = {0, 0},
            sf::Vector2f size = {0, 0}, TextEditColors colors = TextEditColors(),
            int limit = 0, Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

    private:
        Text text;
        TextEditColors colors;

        int limit = 0;

    public:
        TextEdit(const sf::Font& font, sf::Vector2f position = {0, 0},
            sf::Vector2f size = {0, 0}, TextEditColors colors = TextEditColors(),
            int limit = 0, Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

        void handle_event(sf::Event event, sf::Vector2f mouse_pos) override;

        bool is_active() const;

        void set_text(Text text);

        Text& get_text();

        const Text& get_text() const;

        void set_colors(TextEditColors colors);

        TextEditColors get_colors() const;

        void set_limit(int limit);
    };
}
