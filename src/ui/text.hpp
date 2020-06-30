#pragma once

#include <string>
#include <SFML/Graphics/Font.hpp>

#include "widget.hpp"

namespace Ui
{
    using TextStyle = sf::Text::Style;

    class Text : public Widget
    {
    public:
        Text(const sf::Font& font, const std::string& string = "");

        Text& set_string(const std::string& string);

        std::string get_string() const;

        Text& set_font(const sf::Font& font);

        const sf::Font& get_font() const;

        Text& set_max_width(float max_width);

        float get_max_width() const;

        Text& set_color(Color color);

        Color get_color() const;

        Text& set_font_size(unsigned int font_size);

        unsigned int get_font_size() const;

        Text& set_outline_color(Color outline_color);

        Color get_outline_color() const;

        Text& set_outline_thickness(float outline_thickness);

        float get_outline_thickness() const;

        Text& set_style(TextStyle style);

        TextStyle get_style() const;

    private:
        std::string string;
        const sf::Font* font;

        float max_width = 0;
        Color color = Color::Black;
        unsigned int font_size = 24;
        Color outline_color = Color::Black;
        float outline_thickness = 0;
        TextStyle style = TextStyle::Regular;

        void update_size();

        Text* clone() const override;
    };
}
