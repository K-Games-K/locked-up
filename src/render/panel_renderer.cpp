#include "render/panel_renderer.hpp"

PanelRenderer::PanelRenderer(sf::RenderWindow& window)
    : WidgetRenderer(window)
{}

void PanelRenderer::render(const Ui::Panel& panel, const float dt, sf::Vector2f parent_pos, sf::Vector2f parent_size)
{
    sf::Vector2f panel_position = panel.get_global_position(parent_pos, parent_size);
    sf::RectangleShape panel_rect(panel.get_global_size(parent_size));
    panel_rect.setPosition(panel_position);
    panel_rect.setFillColor(panel.get_background_color());
    window.draw(panel_rect);
}

void PanelRenderer::render(const Ui::TexturedPanel& panel, const float dt, sf::Vector2f parent_pos, sf::Vector2f parent_size)
{
    sf::Vector2f panel_position = panel.get_global_position(parent_pos, parent_size);
    sf::Sprite panel_sprite(panel.get_background_texture());
    panel_sprite.setPosition(panel_position);
    window.draw(panel_sprite);
}
