#include "ui/text_edit.hpp"

#include <iostream>

namespace Ui
{
    TextEdit::TextEdit(WidgetType type, const sf::Font& font, sf::Vector2f position,
        sf::Vector2f size, TextEditColors colors, int limit, Anchor origin, Anchor anchor)
        : Widget(type, position, size, origin, anchor),
        text("", font), colors(colors), limit(limit)
    {
        text.set_position({5, 0});
        text.set_origin(Anchor::CenterLeft);
        text.set_anchor(Anchor::CenterLeft);
        text.set_color(colors.text_color);
    }

    TextEdit::TextEdit(const sf::Font& font, sf::Vector2f position, sf::Vector2f size,
        TextEditColors colors, int limit, Anchor origin, Anchor anchor)
        : Widget(WidgetType::TextEdit, position, size, origin, anchor),
        text("", font), colors(colors), limit(limit)
    {
        text.set_position({5, 0});
        text.set_origin(Anchor::CenterLeft);
        text.set_anchor(Anchor::CenterLeft);
        text.set_color(colors.text_color);
    }

    void TextEdit::handle_event(sf::Event event, sf::Vector2f mouse_pos)
    {
        sf::FloatRect widget_box({0, 0}, get_size());

        if(event.type == sf::Event::MouseButtonPressed)
            activated = widget_box.contains(mouse_pos);
        else if(activated && event.type == sf::Event::TextEntered)
        {
            char character = event.text.unicode;
            std::string string = text.get_string();
            if(!std::isprint(character) || (limit > 0 && string.size() >= limit))
                return;

            string += character;
            text.set_string(string);
        }
        else if(activated && event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Backspace)
        {
            std::string string = text.get_string();
            if(string.size() == 0)
                return;

            string.pop_back();
            text.set_string(string);
        }
    }

    bool TextEdit::is_active() const
    {
        return activated;
    }

    void TextEdit::set_text(Text text)
    {
        this->text = text;
    }

    Text& TextEdit::get_text()
    {
        return text;
    }

    const Text& TextEdit::get_text() const
    {
        return text;
    }

    void TextEdit::set_colors(TextEditColors colors)
    {
        this->colors = colors;
    }

    TextEditColors TextEdit::get_colors() const
    {
        return colors;
    }

    void TextEdit::set_limit(int limit)
    {
        this->limit = limit;
    }

    void TextEdit::reset()
    {
        activated = false;
    }
}
