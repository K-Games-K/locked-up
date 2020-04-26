#include "ui/button.hpp"

namespace Ui
{
    Button::Button(WidgetType type, const std::string& text,
        const sf::Font& font, Callback callback, sf::Vector2f position, sf::Vector2f size,
        ButtonColors colors, Ui::Anchor origin, Ui::Anchor anchor)
        : Widget(type, position, size, origin, anchor),
        text(text, font), colors(colors), callback(callback)
    {
        this->text.set_origin(Anchor::Center);
        this->text.set_anchor(Anchor::Center);
    }

    Button::Button(const std::string& text, const sf::Font& font,
        Callback callback, sf::Vector2f position, sf::Vector2f size, ButtonColors colors,
        Ui::Anchor origin, Ui::Anchor anchor)
        : Widget(WidgetType::Button, position, size, origin, anchor),
        text(text, font), colors(colors), callback(callback)
    {
        this->text.set_origin(Anchor::Center);
        this->text.set_anchor(Anchor::Center);
    }

    void Button::handle_event(sf::Event event, sf::Vector2f mouse_pos)
    {
        sf::FloatRect widget_box({0, 0}, get_size());

        if(event.type == sf::Event::MouseButtonPressed &&
            widget_box.contains(mouse_pos))
            activated = true;
        if(event.type == sf::Event::MouseButtonReleased)
            activated = false;
        if(event.type == sf::Event::MouseMoved)
            hovered = widget_box.contains(mouse_pos);
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
