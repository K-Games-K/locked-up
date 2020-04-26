#pragma once

#include <SFML/System/Vector2.hpp>

namespace Ui
{
    /// WidgetType allows to recognize widget using polymorphic type.
    enum class WidgetType
    {
        Text,
        Button,
        Panel,
        UserInterface
    };

    enum class Anchor
    {
        TopLeft,
        TopRight,
        BottomRight,
        BottomLeft,
        Center,
        CenterTop,
        CenterRight,
        CenterBottom,
        CenterLeft
    };

    /// Base class for all UI widgets.
    class Widget
    {
    private:
        const WidgetType type;
        const Widget* parent;

        sf::Vector2f position;
        sf::Vector2f size;

        Anchor origin;
        Anchor anchor;

    public:
        /// This constructor should be called by widgets that don't require parent.
        Widget(WidgetType type, sf::Vector2f position = {0, 0},
            sf::Vector2f size = {0, 0}, Anchor origin = Anchor::TopLeft,
            Anchor anchor = Anchor::TopLeft);

        /// This constructor should be called by widgets that require parent.
        Widget(WidgetType type, const Widget& parent, sf::Vector2f position = {0, 0},
            sf::Vector2f size = {0, 0}, Anchor origin = Anchor::TopLeft,
            Anchor anchor = Anchor::TopLeft);

        virtual ~Widget() = default;

        WidgetType get_type() const;

        void set_parent(const Widget& parent);

        const Widget& get_parent() const;

        void set_position(sf::Vector2f position);

        sf::Vector2f get_position() const;

        void set_size(sf::Vector2f size);

        sf::Vector2f get_size() const;

        void set_origin(Anchor origin);

        Anchor get_origin() const;

        void set_anchor(Anchor anchor);

        Anchor get_anchor() const;

        sf::Vector2f get_absolute_position() const;

        bool operator==(const Widget& other) const;

        Widget& operator=(const Widget& other);
    };
}
