#pragma once

#include <string>
#include <SFML/Graphics/Font.hpp>

#include "widget.hpp"

namespace Ui
{
    struct TextSettings
    {
        sf::Color color = sf::Color::White;
        unsigned int font_size = 24;
        sf::Color outline_color = sf::Color::Black;
        float outline_thickness = 0;
    };

    class Text : public Widget
    {
    private:
        std::string string;
        const sf::Font* font;

        TextSettings settings;

        void update_size();

    public:
        Text(const std::string& text, const sf::Font& font, sf::Vector2f position = {0, 0},
            TextSettings text_settings = TextSettings(), Anchor origin = Anchor::TopLeft,
            Anchor anchor = Anchor::TopLeft);

        void set_string(const std::string& text);

        std::string get_string() const;

        void set_font(const sf::Font& font);

        const sf::Font& get_font() const;

        void set_color(sf::Color color);

        void set_font_size(unsigned int font_size);

        void set_outline_color(sf::Color outline_color);

        void set_outline_thickness(float outline_thickness);

        void set_settings(TextSettings settings);

        TextSettings get_settings() const;
    };
}
