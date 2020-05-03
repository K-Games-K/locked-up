#include <SFML/Graphics/Text.hpp>

#include "ui/text.hpp"

namespace Ui
{
    Text::Text(const sf::Font& font, const std::string& string)
        : font(&font), string(string)
    {
        update_size();
    }

    Text& Text::set_string(const std::string& string)
    {
        this->string = string;
        update_size();

        return *this;
    }

    std::string Text::get_string() const
    {
        return string;
    }

    Text& Text::set_font(const sf::Font& font)
    {
        this->font = &font;
        update_size();

        return *this;
    }

    const sf::Font& Text::get_font() const
    {
        return *font;
    }

    Text& Text::set_max_width(float max_width)
    {
        this->max_width = max_width;
        update_size();

        return *this;
    }

    float Text::get_max_width() const
    {
        return max_width;
    }

    Text& Text::set_color(Color color)
    {
        this->color = color;

        return *this;
    }

    Color Text::get_color() const
    {
        return color;
    }

    Text& Text::set_font_size(unsigned int font_size)
    {
        this->font_size = font_size;
        update_size();

        return *this;
    }

    unsigned int Text::get_font_size() const
    {
        return font_size;
    }

    Text& Text::set_outline_color(Color outline_color)
    {
        this->outline_color = outline_color;

        return *this;
    }

    Color Text::get_outline_color() const
    {
        return outline_color;
    }

    Text& Text::set_outline_thickness(float outline_thickness)
    {
        this->outline_thickness = outline_thickness;

        return *this;
    }

    float Text::get_outline_thickness() const
    {
        return outline_thickness;
    }

    Text& Text::set_style(TextStyle style)
    {
        this->style = style;

        return *this;
    }

    TextStyle Text::get_style() const
    {
        return style;
    }

    void Text::update_size()
    {
        if(max_width > 0)
        {
            std::string str;
            str.reserve(string.size());
            sf::Text sf_text("", *font, font_size);
            for(char ch : string)
            {
                sf_text.setString(str += ch);
                sf::FloatRect bounds = sf_text.getLocalBounds();
                sf::Vector2f size = {bounds.width + bounds.left, bounds.height + bounds.top};

                if(size.x > max_width)
                    str.insert(str.size() - 2, 1, '\n');
            }

            string = str;
        }

        sf::Text sf_text(string, *font, font_size);
        sf::FloatRect bounds = sf_text.getLocalBounds();
        set_size({bounds.width + bounds.left, bounds.height + bounds.top});
    }

    Text* Text::clone() const
    {
        return new Text(*this);
    }
}
