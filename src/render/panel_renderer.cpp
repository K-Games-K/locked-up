#include "panel_renderer.hpp"

PanelRenderer::PanelRenderer(sf::RenderWindow& window)
    : WidgetRenderer(window) {}

void PanelRenderer::render(const Ui::Panel& panel, const float dt, sf::Vector2f parent_pos,
    sf::Vector2f parent_size)
{
    sf::Vector2f panel_position = panel.get_global_position(parent_pos, parent_size);

    if(panel.has_background_texture())
    {
        auto& texture = panel.get_background_texture();
        sf::Vector2f factors = {
            panel.get_size().x / texture.getSize().x,
            panel.get_size().y / texture.getSize().y
        };
        sf::Sprite panel_sprite(texture);
        panel_sprite.setPosition(panel_position);
        panel_sprite.setScale(factors);
        window.draw(panel_sprite);
    }

    sf::RectangleShape panel_rect(panel.get_size());
    panel_rect.setPosition(panel_position);
    panel_rect.setFillColor(panel.get_background_color());
    window.draw(panel_rect);
}
