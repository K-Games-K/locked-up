#include "render/visual_item_renderer.hpp"

VisualItemRenderer::VisualItemRenderer(sf::RenderWindow& window, ResourceManagers resources)
	:WidgetRenderer(window, resources)
{}

void VisualItemRenderer::render(const Ui::VisualItem& item , const float dt)
{
	if (!item.is_enabled())
		return;

	sf::Vector2f button_position = item.get_relative_position(origin_pos, parent_size);
	sf::Sprite item_sprite(item.get_texture());
	item_sprite.setPosition(button_position);
	window.draw(item_sprite);
}