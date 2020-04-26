#pragma once

#include "widget_renderer.hpp"
#include "text_renderer.hpp"
#include "ui/text_edit.hpp"

class TextEditRenderer : public WidgetRenderer<Ui::TextEdit>
{
private:
    TextRenderer text_renderer;

public:
    TextEditRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::TextEdit& text_edit, const float dt) override;
};
