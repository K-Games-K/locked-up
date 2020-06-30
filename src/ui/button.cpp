#include "button.hpp"

namespace Ui
{
    bool Button::handle_event(const sf::Event& event, sf::Vector2f mouse_pos,
        sf::Vector2f parent_pos, sf::Vector2f parent_size)
    {
        sf::Vector2f widget_pos = get_global_position(parent_pos, parent_size);
        sf::Vector2f widget_size = get_size();
        sf::FloatRect widget_box(widget_pos, widget_size);

        if(event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left
            && widget_box.contains(mouse_pos))
        {
            activated = true;
            if(callback)
                callback(*this);

            return true;
        }
        else if(event.type == sf::Event::MouseButtonReleased
            && event.mouseButton.button == sf::Mouse::Left)
        {
            activated = false;
        }
        else if(event.type == sf::Event::MouseMoved)
        {
            hovered = widget_box.contains(mouse_pos);
        }

        return Widget::handle_event(event, mouse_pos, widget_pos, widget_size);
    }

    Button& Button::set_callback(Button::Callback callback)
    {
        this->callback = callback;

        return *this;
    }

    Button& Button::set_default_color(Color default_color)
    {
        this->default_color = default_color;

        return *this;
    }

    Button& Button::set_hover_color(Color hover_color)
    {
        this->hover_color = hover_color;

        return *this;
    }

    Button& Button::set_active_color(Color active_color)
    {
        this->active_color = active_color;

        return *this;
    }

    Color Button::get_color() const
    {
        if(activated)
            return active_color;
        else if(hovered)
            return hover_color;
        else
            return default_color;
    }

    const sf::Texture& Button::get_texture() const
    {
        if(activated)
            return *active_texture;
        else if(hovered)
            return *hover_texture;
        else
            return *default_texture;
    }

    Button& Button::set_default_texture(const sf::Texture& default_texture)
    {
        this->default_texture = &default_texture;

        return *this;
    }

    Button& Button::set_hover_texture(const sf::Texture& hover_texture)
    {
        this->hover_texture = &hover_texture;

        return *this;
    }

    Button& Button::set_active_texture(const sf::Texture& active_texture)
    {
        this->active_texture = &active_texture;

        return *this;
    }

    void Button::reset()
    {
        activated = false;
        hovered = false;

        Widget::reset();
    }

    bool Button::has_texture() const
    {
        return default_texture != nullptr
            && hover_texture != nullptr
            && active_texture != nullptr;
    }

    Button* Button::clone() const
    {
        return new Button(*this);
    }
}
