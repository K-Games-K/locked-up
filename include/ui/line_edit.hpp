#pragma once

#include "widget.hpp"
#include "text.hpp"

namespace Ui
{
    class LineEdit : public Widget
    {
    public:
        LineEdit(const sf::Font& font, const std::string& string = "");

        bool handle_event(const sf::Event& event, sf::Vector2f mouse_pos,
            sf::Vector2f parent_pos, sf::Vector2f parent_size) override;

        bool is_active() const;

        LineEdit& set_text(const std::string& string);

        const Text& get_text() const;

        LineEdit& set_max_length(int max_length);

        int get_max_length() const;

        LineEdit& set_background_color(Color background_color);

        Color get_background_color() const;

        LineEdit& set_border_color(Color border_color);

        Color get_border_color() const;

        LineEdit& set_border_thickness(float border_thickness);

        float get_border_thickness() const;

        LineEdit& set_text_color(Color text_color);

        Color get_text_color() const;

        LineEdit& set_margin(float margin);

        float get_margin() const;

        void reset() override;

    protected:
        bool activated = false;

    private:
        Text text;

        int max_length = 0;

        Color background_color = Color::Black;
        Color border_color = Color::White;
        float border_thickness = 1;
        float margin = 5;

        LineEdit* clone() const override;
    };
}
