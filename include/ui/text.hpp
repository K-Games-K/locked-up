#pragma once

#include <string>
#include <SFML/Graphics/Font.hpp>

#include "widget.hpp"

namespace Ui
{
    class Text : public Widget
    {
    private:
        static constexpr unsigned int DEFAULT_SIZE = 24;

        std::string text;
        const sf::Font* font;
        unsigned int font_size;
        sf::Color color;

        void update_size();

    public:
        Text(const std::string& text, const sf::Font& font, unsigned int font_size = DEFAULT_SIZE,
            sf::Vector2f position = {0, 0}, sf::Color color = sf::Color::White,
            Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

        void set_text(const std::string& text);

        std::string get_text() const;

        void set_font(const sf::Font& font);

        const sf::Font& get_font() const;

        void set_font_size(unsigned int font_size);

        unsigned int get_font_size() const;

        void set_color(sf::Color color);

        sf::Color get_color() const;
    };
}
