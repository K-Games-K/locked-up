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
        Layout(LayoutType type, float spacing);

        Widget* add_widget(const Widget& widget) override;

        void remove_widget(const Widget& widget) override;

        std::vector<Widget*> get_items();

        std::vector<const Widget*> get_items() const;

        void clear();

    private:
        const LayoutType type;

        class Item : public Widget
        {
        private:
            Item* clone() const override;
        };

        float spacing;

        void update_size();

        Layout* clone() const override;
    };
}
