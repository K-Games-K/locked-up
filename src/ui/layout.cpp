#include "utils.hpp"
#include "ui/layout.hpp"

namespace Ui
{
    Layout::Layout(LayoutType type, float margin)
        : type(type), margin(margin) {}

    Widget* Layout::add_widget(const Widget& widget)
    {
        auto result = Widget::add_widget(Item())->add_widget(widget);
        update_size();
        return result;
    }

    void Layout::remove_widget(const Widget& widget)
    {
        get_children().erase(
            std::remove_if(
                get_children().begin(), get_children().end(), [&widget](auto& child) {
                    return *child->get_children()[0] == widget;
                }
            )
        );

        update_size();
    }

    std::vector<Widget*> Layout::get_items()
    {
        std::vector<Widget*> items;
        items.reserve(get_children().size());

        for(auto& child : get_children())
            items.push_back(child->get_child<Widget>());

        return items;
    }

    std::vector<const Widget*> Layout::get_items() const
    {
        std::vector<const Widget*> items;
        items.reserve(get_children().size());

        for(auto& child : get_children())
            items.push_back(child->get_child<Widget>());

        return items;
    }

    void Layout::clear()
    {
        get_children().clear();
    }

    void Layout::update_size()
    {
        switch(type)
        {
            case LayoutType::Horizontal:
            {
                float layout_width = 0;
                float layout_height = 0;
                for(auto& child : get_children())
                {
                    auto item = child->get_child<>(0);
                    child->set_position({layout_width, 0})
                        .set_size(item->get_size())
                        .set_origin(Origin::CenterLeft)
                        .set_anchor(Anchor::CenterLeft);
                    layout_width += child->get_size().x + margin;
                    layout_height = Utils::max(layout_height, child->get_size().y);
                }
                set_size({layout_width, layout_height});

                break;
            }
            case LayoutType::Vertical:
            {
                float layout_width = 0;
                float layout_height = 0;
                for(auto& child : get_children())
                {
                    auto item = child->get_child<>(0);
                    child->set_position({0, layout_height})
                        .set_size(item->get_size())
                        .set_origin(Origin::CenterTop)
                        .set_anchor(Anchor::CenterTop);
                    layout_width = Utils::max(layout_width, child->get_size().x);
                    layout_height += child->get_size().y + margin;
                }
                set_size({layout_width, layout_height});

                break;
            }
        }
    }

    Layout* Layout::clone() const
    {
        return new Layout(*this);
    }

    Layout::Item* Layout::Item::clone() const
    {
        return new Item(*this);
    }
}
