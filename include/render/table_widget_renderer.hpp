#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "button_renderer.hpp"
#include "text_edit_renderer.hpp"
#include "checkbox_renderer.hpp"
#include "ui/table_widget.hpp"

class TableWidgetRenderer : public WidgetRenderer<Ui::TableWidget>
{
private:
    TextRenderer text_renderer;
    ButtonRenderer button_renderer;
    TextEditRenderer text_edit_renderer;
    CheckboxRender checkbox_render;

public:
    TableWidgetRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::TableWidget& table_widget, const float dt) override;
};
