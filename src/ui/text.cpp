#include <SFML/Graphics/Text.hpp>

#include "ui/text.hpp"

namespace Ui
{
    Text::Text(const std::string& text, const sf::Font& font, unsigned int font_size,
        sf::Vector2f position, sf::Color color, Ui::Anchor origin, Ui::Anchor anchor)
        : Widget(WidgetType::Text, position, {0, 0}, origin, anchor),
        string(text), font(&font), font_size(font_size), color(color)
    {
        update_size();
    }

    void Text::set_string(const std::string& text)
    {
        this->string = text;
        update_size();
    }

    std::string Text::get_string() const
    {
        return string;
    }

    void Text::set_font(const sf::Font& font)
    {
        this->font = &font;
    }

    const sf::Font& Text::get_font() const
    {
        return *font;
    }

    void Text::set_font_size(unsigned int font_size)
    {
        this->font_size = font_size;
        update_size();
    }

    unsigned int Text::get_font_size() const
    {
        return font_size;
    }

    void Text::set_color(sf::Color color)
    {
        this->color = color;
    }

    sf::Color Text::get_color() const
    {
        return color;
    }

    void Text::update_size()
    {
        sf::Text sf_text(string, *font, font_size);
        sf::FloatRect bounds = sf_text.getLocalBounds();
        set_size({bounds.width + bounds.left, (float) font_size});
    }
}
