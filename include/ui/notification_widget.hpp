#pragma once

#include "widget.hpp"
#include "text.hpp"

namespace Ui
{
    class NotificationWidget : public Widget
    {
    public:
        NotificationWidget(const sf::Font& font);

        void update(const float dt) override;

        void show(const std::string& string, float fadeout_time = 2.5);

        Text& get_text();

    private:
        float fadeout_time = 0;

        NotificationWidget* clone() const override;
    };
}
