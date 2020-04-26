#include <stdexcept>

#include "ui/widget.hpp"

namespace Ui
{
    Widget::Widget(WidgetType type, sf::Vector2f position, sf::Vector2f size,
        Anchor origin, Anchor anchor)
        : type(type), parent(nullptr), position(position), size(size), origin(origin), anchor(anchor)
    {}

    Widget::Widget(WidgetType type, const Widget& parent, sf::Vector2f position, sf::Vector2f size,
        Anchor origin, Anchor anchor)
        : type(type), parent(&parent), position(position), size(size), origin(origin), anchor(anchor)
    {}

    WidgetType Widget::get_type() const
    {
        return type;
    }

    void Widget::set_parent(const Widget& parent)
    {
        this->parent = &parent;
    }

    const Widget& Widget::get_parent() const
    {
        return *parent;
    }

    void Widget::set_position(sf::Vector2f position)
    {
        this->position = position;
    }

    sf::Vector2f Widget::get_position() const
    {
        return position;
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

    sf::Vector2f Widget::get_absolute_position() const
    {
        sf::Vector2f result = parent != nullptr ? parent->get_absolute_position() : sf::Vector2f();
        switch(origin)
        {
            case Anchor::TopLeft:
                result += position;
                break;
            case Anchor::TopRight:
                result += position - sf::Vector2f(size.x, 0);
                break;
            case Anchor::BottomRight:
                result += position - size;
                break;
            case Anchor::BottomLeft:
                result += position - sf::Vector2f(0, size.y);
                break;
            case Anchor::Center:
                result += position - size / 2.0f;
                break;
            case Anchor::CenterTop:
                result += position - sf::Vector2f(size.x / 2, 0);
                break;
            case Anchor::CenterRight:
                result += position - sf::Vector2f(size.x, size.y / 2);
                break;
            case Anchor::CenterBottom:
                result += position - sf::Vector2f(size.x / 2, size.y);
                break;
            case Anchor::CenterLeft:
                result += position - sf::Vector2f(0, size.y / 2);
                break;
            default:
                break;
        }

        if(parent == nullptr)
            return result;

        switch(anchor)
        {
            case Anchor::TopLeft:
                break;
            case Anchor::TopRight:
                result += sf::Vector2f(parent->size.x, 0);
                break;
            case Anchor::BottomRight:
                result += parent->size;
                break;
            case Anchor::BottomLeft:
                result += sf::Vector2f(0, parent->size.y);
                break;
            case Anchor::Center:
                result += parent->size / 2.0f;
                break;
            case Anchor::CenterTop:
                result += sf::Vector2f(parent->size.x / 2, 0);
                break;
            case Anchor::CenterRight:
                result += sf::Vector2f(parent->size.x, parent->size.y / 2);
                break;
            case Anchor::CenterBottom:
                result += sf::Vector2f(parent->size.x / 2, parent->size.y);
                break;
            case Anchor::CenterLeft:
                result += sf::Vector2f(0, parent->size.y / 2);
                break;
            default:
                break;
        }

        return result;
    }

    bool Widget::operator==(const Widget& other) const
    {
        return this == &other;
    }

    Widget& Widget::operator=(const Widget& other)
    {
        if(type != other.type)
            throw std::invalid_argument("Tried to assign widgets of different types!");

        parent = other.parent;
        position = other.position;
        size = other.size;
        origin = other.origin;
        anchor = other.anchor;

        return *this;
    }
}
