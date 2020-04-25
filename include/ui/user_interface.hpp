#pragma once

#include <memory>
#include <vector>

#include "widget.hpp"

namespace Ui
{
    class UserInterface : public Widget
    {
    private:
        std::vector<std::unique_ptr<Widget>> widgets;

    public:
        UserInterface(sf::Vector2f position, sf::Vector2f size);

        void update();

        void add_widget(std::unique_ptr<Widget>&& widget);

        const std::vector<std::unique_ptr<Widget>>& get_widgets() const;

        sf::Vector2f get_absolute_position() const override;
    };
}
