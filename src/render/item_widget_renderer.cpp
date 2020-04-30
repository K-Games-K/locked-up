#include "render/item_widget_renderer.hpp"

ItemWidgetRenderer::ItemWidgetRenderer(sf::RenderWindow& window, ResourceManagers resources)
	:WidgetRenderer(window, resources)
{}

void ItemWidgetRenderer::render(const std::vector<Ui::ItemWidget>& items , const float dt)
{
    for(auto& item : items)
    {
        if (!item.is_enabled())
            return;

        sf::Vector2f item_pos = item.get_relative_position(origin_pos, parent_size);
        sf::Sprite item_sprite(item.get_texture());
        item_sprite.setPosition(item_pos);
        window.draw(item_sprite);
    }
}
