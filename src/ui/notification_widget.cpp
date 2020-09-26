#include "utils.hpp"
#include "ui/notification_widget.hpp"

namespace Ui
{
    Ui::NotificationWidget::NotificationWidget(const sf::Font& font)
    {
        Widget::add_widget(Text(font));
    }

    void NotificationWidget::update(const float dt)
    {
        Text& text = get_text();

        fadeout_time = Utils::max(fadeout_time - dt, 0);
        if(fadeout_time < 0.5)
        {
            Ui::Color color = text.get_color();
            Ui::Color outline_color = text.get_outline_color();
            color.a = 255 * (fadeout_time / 0.5);
            outline_color.a = 255 * (fadeout_time / 0.5);
            text.set_color(color)
                .set_outline_color(outline_color);
        }
        if(fadeout_time <= 0)
            set_enabled(false);

        Widget::update(dt);
    }

    void NotificationWidget::show(const std::string& string, float fadeout_time)
    {
        this->fadeout_time = fadeout_time;
        get_text().set_string(string);
        set_enabled(true);
    }

    Text& NotificationWidget::get_text()
    {
        return *get_child<Text>();
    }

    NotificationWidget* NotificationWidget::clone() const
    {
        return new NotificationWidget(*this);
    }
}
