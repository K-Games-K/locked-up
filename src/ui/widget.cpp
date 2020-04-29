#include <stdexcept>

#include "ui/widget.hpp"

namespace Ui
{
    Widget::Widget(sf::Vector2f position, sf::Vector2f size,
        Anchor origin, Anchor anchor)
        : position(position), size(size), origin(origin), anchor(anchor)
    {}

    void Widget::set_enabled(bool enabled)
    {
        this->enabled = enabled;
        if(!enabled)
            this->reset();
    }

    bool Widget::is_enabled() const
    {
        return enabled;
    }

    void Widget::set_position(sf::Vector2f position)
    {
        this->position = position;
    }

    sf::Vector2f Widget::get_local_position() const
    {
        return position;
    }

    sf::Vector2f
    Widget::get_relative_position(sf::Vector2f origin_pos, sf::Vector2f parent_size) const
    {
        sf::Vector2f result = origin_pos + position;
        switch(origin)
        {
            case Anchor::TopLeft:
                break;
            case Anchor::TopRight:
                result -= sf::Vector2f(size.x, 0);
                break;
            case Anchor::BottomRight:
                result -= size;
                break;
            case Anchor::BottomLeft:
                result -= sf::Vector2f(0, size.y);
                break;
            case Anchor::Center:
                result -= size / 2.0f;
                break;
            case Anchor::CenterTop:
                result -= sf::Vector2f(size.x / 2, 0);
                break;
            case Anchor::CenterRight:
                result -= sf::Vector2f(size.x, size.y / 2);
                break;
            case Anchor::CenterBottom:
                result -= sf::Vector2f(size.x / 2, size.y);
                break;
            case Anchor::CenterLeft:
                result -= sf::Vector2f(0, size.y / 2);
                break;
            default:
                break;
        }

        switch(anchor)
        {
            case Anchor::TopLeft:
                break;
            case Anchor::TopRight:
                result += sf::Vector2f(parent_size.x, 0);
                break;
            case Anchor::BottomRight:
                result += parent_size;
                break;
            case Anchor::BottomLeft:
                result += sf::Vector2f(0, parent_size.y);
                break;
            case Anchor::Center:
                result += parent_size / 2.0f;
                break;
            case Anchor::CenterTop:
                result += sf::Vector2f(parent_size.x / 2, 0);
                break;
            case Anchor::CenterRight:
                result += sf::Vector2f(parent_size.x, parent_size.y / 2);
                break;
            case Anchor::CenterBottom:
                result += sf::Vector2f(parent_size.x / 2, parent_size.y);
                break;
            case Anchor::CenterLeft:
                result += sf::Vector2f(0, parent_size.y / 2);
                break;
            default:
                break;
        }

        return result;
    }

    void Widget::set_size(sf::Vector2f size)
    {
        this->size = size;
    }

    sf::Vector2f Widget::get_size() const
    {
        return size;
    }

    void Widget::set_origin(Anchor origin)
    {
        this->origin = origin;
    }

    Anchor Widget::get_origin() const
    {
        return origin;
    }

    void Widget::set_anchor(Anchor anchor)
    {
        this->anchor = anchor;
    }

    Anchor Widget::get_anchor() const
    {
        return anchor;
    }

    bool Widget::operator==(const Widget& other) const
    {
        return this == &other;
    }
}
