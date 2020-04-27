#pragma once

#include <SFML/Graphics.hpp>

namespace Ui
{
    /// WidgetType allows to recognize widget using polymorphic type.
    enum class WidgetType
    {
        Text,
        Button,
        TexturedButton,
        TextEdit,
        TexturedTextEdit,
        Panel,
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

        bool enabled = true;

        sf::Vector2f position;
        sf::Vector2f size;

        Anchor origin;
        Anchor anchor;

    public:
        Widget(WidgetType type, sf::Vector2f position = {0, 0},
            sf::Vector2f size = {0, 0}, Anchor origin = Anchor::TopLeft,
            Anchor anchor = Anchor::TopLeft);

        virtual ~Widget() = default;

        virtual void handle_event(sf::Event event, sf::Vector2f mouse_pos) {};

        WidgetType get_type() const;

        void set_enabled(bool enabled);

        bool is_enabled() const;

        void set_position(sf::Vector2f position);

        sf::Vector2f get_local_position() const;

        sf::Vector2f get_relative_position(sf::Vector2f origin_pos, sf::Vector2f parent_size) const;

        void set_size(sf::Vector2f size);

        sf::Vector2f get_size() const;

        void set_origin(Anchor origin);

        Anchor get_origin() const;

        void set_anchor(Anchor anchor);

        Anchor get_anchor() const;

        bool operator==(const Widget& other) const;

        Widget& operator=(const Widget& other);
    };
}
