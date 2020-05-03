#pragma once

#include <functional>

#include "widget.hpp"
#include "text.hpp"

namespace Ui
{
    class Button : public Widget
    {
    public:
        using Callback = std::function<void(Button&)>;

        bool handle_event(const sf::Event& event, sf::Vector2f mouse_pos,
            sf::Vector2f parent_pos, sf::Vector2f parent_size) override;

        Button& set_callback(Callback callback);

        Button& set_default_color(Color default_color);

        Button& set_hover_color(Color hover_color);

        Button& set_active_color(Color active_color);

        Color get_color() const;

        void reset() override;

    protected:
        bool activated = false;
        bool hovered = false;

    private:
        Callback callback;

        Color default_color = Color::Transparent;
        Color hover_color = Color::Transparent;
        Color active_color = Color::Transparent;

        Button* clone() const override;
    };
}
