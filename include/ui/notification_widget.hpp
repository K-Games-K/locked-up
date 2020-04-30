#pragma once

#include "widget.hpp"
#include "text.hpp"

namespace Ui
{
    class NotificationWidget : public Widget
    {
    private:
        Text text;

        float fadeout_time = 0;

    public:
        NotificationWidget(const sf::Font& font, sf::Vector2f position = {0, 0},
            sf::Vector2f size = {0, 0}, TextSettings text_settings = TextSettings(),
            Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft);

        void update(const float dt) override;

        void show_notification(const std::string& string, float fadeout_time);

        const Text& get_text() const;

        float get_fadeout_time() const;
    };
}
