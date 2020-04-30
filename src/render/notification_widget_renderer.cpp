#include "render/notification_widget_renderer.hpp"

NotificationWidgetRenderer::NotificationWidgetRenderer(sf::RenderWindow& window,
    ResourceManagers resources)
    : WidgetRenderer(window, resources),
    text_renderer(window, resources)
{}

void NotificationWidgetRenderer::render(const Ui::NotificationWidget& notification_widget,
    const float dt)
{
    if(!notification_widget.is_enabled())
        return;

    float time = notification_widget.get_fadeout_time();
    if(time <= 0)
        return;

    sf::Vector2f notification_pos = notification_widget.get_relative_position(
        origin_pos, parent_size
    );

    Ui::Text notification_text = notification_widget.get_text();
    if(time < 0.5)
    {
        Ui::TextSettings text_settings = notification_text.get_settings();
        text_settings.color.a = 255 * (time / 0.5);
        text_settings.outline_color.a = 255 * (time / 0.5);
        notification_text.set_settings(text_settings);
    }

    text_renderer.set_origin_pos(notification_pos);
    text_renderer.set_parent_size(notification_widget.get_size());
    text_renderer.render(notification_text, dt);
}
