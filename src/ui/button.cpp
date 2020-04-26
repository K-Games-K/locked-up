#include "ui/button.hpp"

namespace Ui
{
    Button::Button(const Widget& parent, const std::string& text, const sf::Font& font,
        Callback callback, sf::Vector2f position, sf::Vector2f size, ButtonColors colors,
        Ui::Anchor origin, Ui::Anchor anchor)
        : Widget(WidgetType::Button, parent, position, size, origin, anchor),
        text(*this, text, font), colors(colors), callback(callback)
    {
        this->text.set_origin(Anchor::Center);
        this->text.set_anchor(Anchor::Center);
    }

    void Button::set_activated(bool activated)
    {
        this->activated = activated;
        if(activated)
            callback(*this);
    }

    void Button::set_hovered(bool hovered)
    {
        this->hovered = hovered;
    }

    void Button::set_text(Text text)
    {
        this->text = text;
    }

    const Text& Button::get_text() const
    {
        return text;
    }

    void Button::set_colors(ButtonColors colors)
    {
        this->colors = colors;
    }

    ButtonColors Button::get_colors() const
    {
        return colors;
    }

    sf::Color Button::get_color() const
    {
        if(activated)
            return colors.active_color;
        else if(hovered)
            return colors.hover_color;
        else
            return colors.default_color;
    }

    void Button::set_callback(Button::Callback callback)
    {
        this->callback = callback;
    }
}
