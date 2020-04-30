#include "utils.hpp"
#include "ui/notification_widget.hpp"

namespace Ui
{
    Ui::NotificationWidget::NotificationWidget(const sf::Font& font, sf::Vector2f position,
        sf::Vector2f size, TextSettings text_settings, Anchor origin, Anchor anchor)
        : Widget(position, size, origin, anchor),
        text("", font, {0, 0}, text_settings, Anchor::Center, Anchor::Center)
    {}

    void NotificationWidget::update(const float dt)
    {
        fadeout_time = Utils::max(fadeout_time - dt, 0);
    }

    void NotificationWidget::show_notification(const std::string& string, float fadeout_time)
    {
        this->fadeout_time = fadeout_time;
        text.set_string(string);
    }

    const Text& NotificationWidget::get_text() const
    {
        return text;
    }

    float NotificationWidget::get_fadeout_time() const
    {
        return fadeout_time;
    }
}
