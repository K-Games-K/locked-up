
#include "ui/visual_item.hpp"

Visual_item::Visual_item(const std::string& name, const sf::Texture& texture, sf::Vector2f position,
    Anchor origin = Anchor::TopLeft, Anchor anchor = Anchor::TopLeft)
    :Wiget(position, {0,0}, origin, anchor)
{}

void Visual_item::set_texture(sf::Texture texture)
{
    this->texture = texture;
}
