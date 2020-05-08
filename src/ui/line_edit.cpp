#include "ui/line_edit.hpp"

namespace Ui
{
    LineEdit::LineEdit(const sf::Font& font, const std::string& string)
        : text(font, string)
    {
        text.set_color(Color::White)
            .set_origin(Origin::CenterLeft)
            .set_anchor(Anchor::CenterLeft);
    }

    bool LineEdit::handle_event(const sf::Event& event, sf::Vector2f mouse_pos,
        sf::Vector2f parent_pos, sf::Vector2f parent_size)
    {
        sf::Vector2f widget_pos = get_global_position(parent_pos, parent_size);
        sf::Vector2f widget_size = get_size();
        sf::FloatRect widget_box(widget_pos, widget_size);

        if(event.type == sf::Event::MouseButtonPressed)
        {
            activated = widget_box.contains(mouse_pos);
        }
        else if(activated && event.type == sf::Event::TextEntered)
        {
            char character = event.text.unicode;
            std::string string = text.get_string();
            if(std::isprint(character) && (max_length <= 0 || string.size() <= max_length))
            {
                string += character;
                text.set_string(string);
            }

            return true;
        }
        else if(activated && event.type == sf::Event::KeyPressed
            && event.key.code == sf::Keyboard::Backspace)
        {
            std::string string = text.get_string();
            if(string.size() > 0)
            {
                string.pop_back();
                text.set_string(string);
            }

            return true;
        }

        return Widget::handle_event(event, mouse_pos, widget_pos, widget_size);
    }

    bool LineEdit::is_active() const
    {
        return activated;
    }

    LineEdit& LineEdit::set_text(const std::string& string)
    {
        text.set_string(string);

        return *this;
    }

    const Text& LineEdit::get_text() const
    {
        return text;
    }

    LineEdit& LineEdit::set_max_length(int max_length)
    {
        this->max_length = max_length;

        return *this;
    }

    int LineEdit::get_max_length() const
    {
        return max_length;
    }

    void LineEdit::reset()
    {
        activated = false;

        Widget::reset();
    }

    LineEdit& LineEdit::set_background_color(Color background_color)
    {
        this->background_color = background_color;

        return *this;
    }

    Color LineEdit::get_background_color() const
    {
        return background_color;
    }

    LineEdit& LineEdit::set_border_color(Color border_color)
    {
        this->border_color = border_color;

        return *this;
    }

    Color LineEdit::get_border_color() const
    {
        return border_color;
    }

    LineEdit& LineEdit::set_border_thickness(float border_thickness)
    {
        this->border_thickness = border_thickness;

        return *this;
    }

    float LineEdit::get_border_thickness() const
    {
        return border_thickness;
    }

    LineEdit& LineEdit::set_text_color(Color text_color)
    {
        text.set_color(text_color);

        return *this;
    }

    Color LineEdit::get_text_color() const
    {
        return text.get_color();
    }

    LineEdit& LineEdit::set_margin(float margin)
    {
        this->margin = margin;

        return *this;
    }

    float LineEdit::get_margin() const
    {
        return margin;
    }

    LineEdit* LineEdit::clone() const
    {
        return new LineEdit(*this);
    }
}
