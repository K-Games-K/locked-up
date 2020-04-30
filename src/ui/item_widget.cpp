
#include "ui/item_widget.hpp"

namespace Ui
{
    ItemWidget::ItemWidget(Item item, const sf::Texture& texture, sf::Vector2f position,
        Anchor origin, Anchor anchor)
        : Widget(position, { 0,0 }, origin, anchor), texture(&texture), item(item)
    {}

    void ItemWidget::set_texture(const sf::Texture& texture)
    {
        this->texture = &texture;
    }

    const sf::Texture& ItemWidget::get_texture() const
    {
        return *texture;
    }
    const Item& ItemWidget::get_item() const
    {
        return item;
    }
}
