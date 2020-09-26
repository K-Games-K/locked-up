#pragma once

#include "panel.hpp"

namespace Ui
{
    enum class LayoutType
    {
        Horizontal,
        Vertical
    };

    class Layout : public Widget
    {
    public:
        Layout(LayoutType type, float margin = 0);

        Widget* add_widget(const Widget& widget) override;

        void remove_widget(const Widget& widget) override;

        std::vector<Widget*> get_items();

        std::vector<const Widget*> get_items() const;

        void clear();

        void update_size();

    private:
        const LayoutType type;

        float margin;

        class Item : public Widget
        {
        private:
            Item* clone() const override;
        };

        Layout* clone() const override;
    };
}
