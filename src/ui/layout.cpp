#include "utils.hpp"
#include "ui/layout.hpp"

namespace Ui
{
    Layout::Layout(LayoutType type, float spacing)
        : type(type), spacing(spacing) {}

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
                auto& children = get_children();
                for(int i = 0; i < children.size(); ++i)
                {
                    children[i]->set_position({spacing * (i + 0.5f), 0})
                        .set_size({spacing, 0})
                        .set_anchor(Anchor::CenterLeft);
                }
                set_size({children.size() * spacing, 0});

                break;
            }
            case LayoutType::Vertical:
            {
                auto& children = get_children();
                for(int i = 0; i < children.size(); ++i)
                {
                    children[i]->set_position({0, spacing * (i + 0.5f)})
                        .set_size({0, spacing})
                        .set_anchor(Anchor::CenterTop);
                }
                set_size({0, children.size() * spacing});

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
