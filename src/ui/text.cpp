#include <SFML/Graphics/Text.hpp>

#include "ui/text.hpp"

namespace Ui
{
    Text::Text(const std::string& text, const sf::Font& font, sf::Vector2f position,
        TextSettings settings, Anchor origin, Anchor anchor)
        : Widget(position, {0, 0}, origin, anchor),
        string(text), font(&font), settings(settings)
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

    void Text::set_color(sf::Color color)
    {
        settings.color = color;
    }

    void Text::set_font_size(unsigned int font_size)
    {
        settings.font_size = font_size;
        update_size();
    }

    void Text::set_outline_color(sf::Color outline_color)
    {
        settings.outline_color = outline_color;
    }

    void Text::set_outline_thickness(float outline_thickness)
    {
        settings.outline_thickness = outline_thickness;
    }

    void Text::set_settings(TextSettings settings)
    {
        this->settings = settings;
    }

    TextSettings Text::get_settings() const
    {
        return settings;
    }

    void Text::update_size()
    {
        sf::Text sf_text(string, *font, settings.font_size);
        sf::FloatRect bounds = sf_text.getLocalBounds();
        set_size({bounds.width + bounds.left, (float) settings.font_size});
    }
}
