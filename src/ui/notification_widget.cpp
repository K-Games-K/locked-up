#include "utils.hpp"
#include "ui/notification_widget.hpp"

namespace Ui
{
    Ui::NotificationWidget::NotificationWidget(const sf::Font& font)
        : text(font)
    {}

    void NotificationWidget::update(const float dt)
    {
        fadeout_time = Utils::max(fadeout_time - dt, 0);
        if(fadeout_time <= 0)
            set_enabled(false);

        Widget::update(dt);
    }

    void NotificationWidget::show(const std::string& string, float fadeout_time)
    {
        this->fadeout_time = fadeout_time;
        text.set_string(string);
        set_enabled(true);
    }

    Text& NotificationWidget::get_text()
    {
        return text;
    }

    const Text& NotificationWidget::get_text() const
    {
        return text;
    }

    float NotificationWidget::get_fadeout_time() const
    {
        return fadeout_time;
    }

    NotificationWidget* NotificationWidget::clone() const
    {
        return new NotificationWidget(*this);
    }
}
