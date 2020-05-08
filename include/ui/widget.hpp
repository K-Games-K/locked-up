#pragma once

#include <SFML/Graphics.hpp>

namespace Ui
{
    enum class Origin
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

    using Anchor = Origin;
    using Color = sf::Color;

    /// Base class for all UI widgets.
    class Widget
    {
    public:
        Widget() = default;

        Widget(const Widget& other);

        virtual bool handle_event(const sf::Event& event, sf::Vector2f mouse_pos,
            sf::Vector2f parent_pos, sf::Vector2f parent_size);

        virtual void update(const float dt);

        /// Returns pointer to newly added child.
        virtual Widget* add_widget(const Widget& widget);

        virtual void remove_widget(const Widget& widget);

        /// Returns pointer to first child of type T.
        template <typename T = Widget>
        T* get_child()
        {
            for(auto& child : children)
                return dynamic_cast<T*>(child.get());

            return nullptr;
        }

        /// Returns pointer to nth child and casts it to type T.
        /// Returns nullptr if there is no such child.
        template <typename T = Widget>
        T* get_child(size_t n)
        {
            if(n >= children.size())
                return nullptr;

            return dynamic_cast<T*>(children[n].get());
        }

        std::vector<std::unique_ptr<Widget>>& get_children();

        const std::vector<std::unique_ptr<Widget>>& get_children() const;

        /// Returns vector of pointers to all children of type T.
        template <typename T>
        std::vector<T*> get_children() const
        {
            std::vector<T*> result;
            for(auto& child : children)
                if(auto ptr = dynamic_cast<T*>(child.get()))
                    result.push_back(ptr);

            return std::move(result);
        }

        Widget& set_enabled(bool enabled);

        bool is_enabled() const;

        Widget& set_position(sf::Vector2f position, bool position_relative = false);

        Widget& move(sf::Vector2f offset);

        sf::Vector2f get_local_position() const;

        sf::Vector2f
        get_global_position(sf::Vector2f parent_pos, sf::Vector2f parent_size) const;

        Widget& set_size(sf::Vector2f size);

        sf::Vector2f get_size() const;

        Widget& set_origin(Origin origin);

        Anchor get_origin() const;

        Widget& set_anchor(Anchor anchor);

        Anchor get_anchor() const;

        virtual void reset();

        bool operator==(const Widget& other) const;

        Widget& operator=(const Widget& other);

        virtual Widget* clone() const = 0;

    private:
        bool enabled = true;

        sf::Vector2f position = {0, 0};
        bool position_relative = false;
        sf::Vector2f size = {0, 0};

        Origin origin = Origin::Center;
        Anchor anchor = Origin::Center;

        std::vector<std::unique_ptr<Widget>> children;
    };
}
