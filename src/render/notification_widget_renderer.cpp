#include "render/notification_widget_renderer.hpp"

NotificationWidgetRenderer::NotificationWidgetRenderer(sf::RenderWindow& window)
    : WidgetRenderer(window), text_renderer(window)
{}

void NotificationWidgetRenderer::render(const Ui::NotificationWidget& notification_widget,
    const float dt, sf::Vector2f parent_pos, sf::Vector2f parent_size)
{
    float time = notification_widget.get_fadeout_time();
    if(time <= 0)
        return;

    sf::Vector2f notification_pos = notification_widget.get_global_position(
        parent_pos, parent_size
    );
    sf::Vector2f notification_size = notification_widget.get_global_size(parent_size);

    Ui::Text notification_text = notification_widget.get_text();
    if(time < 0.5)
    {
        Ui::Color color = notification_text.get_color();
        Ui::Color outline_color = notification_text.get_outline_color();
        color.a = 255 * (time / 0.5);
        outline_color.a = 255 * (time / 0.5);
        notification_text.set_color(color)
            .set_outline_color(outline_color);
    }

    text_renderer.render(notification_text, dt, notification_pos, notification_size);
}
