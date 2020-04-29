
#include "ui/visual_item.hpp"

namespace Ui
{
    VisualItem::VisualItem(Item item, const sf::Texture& texture, sf::Vector2f position,
        Anchor origin, Anchor anchor)
        :Widget(position, { 0,0 }, origin, anchor), texture(&texture), item(item)
    {}

    void VisualItem::set_texture(const sf::Texture& texture)
    {
        this->texture = &texture;
    }

    const sf::Texture& VisualItem::get_texture() const
    {
        return *texture;
    }
    const Item& VisualItem::get_item() const
    {
        return item;
    }
}