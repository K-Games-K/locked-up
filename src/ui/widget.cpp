#include "ui/widget.hpp"

namespace Ui
{
    Widget::Widget(const Widget& other)
        : enabled(other.enabled), position(other.position),
        position_relative(other.position_relative), size(other.size),
        size_relative(other.size_relative), origin(other.origin), anchor(other.anchor)
    {
        children.reserve(other.children.size());
        for(auto& child : other.children)
            children.push_back(std::unique_ptr<Widget>(child->clone()));
    }

    bool Widget::handle_event(const sf::Event& event, sf::Vector2f mouse_pos,
        sf::Vector2f parent_pos, sf::Vector2f parent_size)
    {
        for(auto it = children.rbegin(); it != children.rend(); ++it)
        {
            auto& child = *it;
            if(child != nullptr && child->is_enabled())
            {
                if(child->handle_event(
                    event, mouse_pos,
                    get_global_position(parent_pos, parent_size),
                    get_global_size(parent_size)
                ))
                    return true;
            }
        }

        return false;
    }

    void Widget::update(const float dt)
    {
        for(auto& child : children)
            if(child != nullptr && child->is_enabled())
                child->update(dt);
    }

    Widget* Widget::add_widget(const Widget& widget)
    {
        auto new_widget = std::unique_ptr<Widget>(widget.clone());
        children.push_back(std::move(new_widget));

        return children[children.size() - 1].get();
    }

    void Widget::remove_widget(const Widget& widget)
    {
        children.erase(
            std::find_if(
                children.begin(), children.end(), [&widget](auto& child) {
                    return *child == widget;
                }
            )
        );
    }

    std::vector<std::unique_ptr<Widget>>& Widget::get_children()
    {
        return children;
    }

    const std::vector<std::unique_ptr<Widget>>& Widget::get_children() const
    {
        return children;
    }

    Widget& Widget::set_enabled(bool enabled)
    {
        this->enabled = enabled;
        if(!enabled)
            this->reset();

        return *this;
    }

    bool Widget::is_enabled() const
    {
        return enabled;
    }

    Widget& Widget::set_position(sf::Vector2f position, bool position_relative)
    {
        this->position = position;
        this->position_relative = position_relative;

        return *this;
    }

    Widget& Widget::move(sf::Vector2f offset)
    {
        this->position += offset;

        return *this;
    }

    sf::Vector2f Widget::get_local_position() const
    {
        return position;
    }

    sf::Vector2f
    Widget::get_global_position(sf::Vector2f parent_pos, sf::Vector2f parent_size) const
    {
        sf::Vector2f widget_pos = position_relative ? sf::Vector2f(
            position.x * parent_size.x,
            position.y * parent_size.y
        ) : position;
        sf::Vector2f widget_size = size_relative ? sf::Vector2f(
            size.x * parent_size.x,
            size.y * parent_size.y
        ) : size;

        sf::Vector2f result = parent_pos + widget_pos;
        switch(origin)
        {
            case Origin::TopLeft:
                break;
            case Origin::TopRight:
                result -= sf::Vector2f(widget_size.x, 0);
                break;
            case Origin::BottomRight:
                result -= widget_size;
                break;
            case Origin::BottomLeft:
                result -= sf::Vector2f(0, widget_size.y);
                break;
            case Origin::Center:
                result -= widget_size / 2.0f;
                break;
            case Origin::CenterTop:
                result -= sf::Vector2f(widget_size.x / 2, 0);
                break;
            case Origin::CenterRight:
                result -= sf::Vector2f(widget_size.x, widget_size.y / 2);
                break;
            case Origin::CenterBottom:
                result -= sf::Vector2f(widget_size.x / 2, widget_size.y);
                break;
            case Origin::CenterLeft:
                result -= sf::Vector2f(0, widget_size.y / 2);
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

    Widget& Widget::set_size(sf::Vector2f size, bool size_relative)
    {
        this->size = size;
        this->size_relative = size_relative;

        return *this;
    }

    sf::Vector2f Widget::get_local_size() const
    {
        return size;
    }

    sf::Vector2f Widget::get_global_size(sf::Vector2f parent_size) const
    {
        return size_relative ? sf::Vector2f(
            size.x * parent_size.x,
            size.y * parent_size.y
        ) : size;
    }

    Widget& Widget::set_origin(Anchor origin)
    {
        this->origin = origin;

        return *this;
    }

    Anchor Widget::get_origin() const
    {
        return origin;
    }

    Widget& Widget::set_anchor(Anchor anchor)
    {
        this->anchor = anchor;

        return *this;
    }

    Anchor Widget::get_anchor() const
    {
        return anchor;
    }

    void Widget::reset()
    {
        for(auto& child : children)
            if(child != nullptr)
                child->reset();
    }

    bool Widget::operator==(const Widget& other) const
    {
        return this == &other;
    }

    Widget& Widget::operator=(const Widget& other)
    {
        enabled = other.enabled;
        position = other.position;
        position_relative = other.position_relative;
        size = other.size;
        size_relative = other.size_relative;
        origin = other.origin;
        anchor = other.anchor;

        children.reserve(other.children.size());
        for(auto& child : other.children)
            children.push_back(std::unique_ptr<Widget>(child->clone()));

        return *this;
    }
}
