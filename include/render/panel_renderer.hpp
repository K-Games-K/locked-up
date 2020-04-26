#pragma once

#include "renderer.hpp"
#include "text_renderer.hpp"
#include "button_renderer.hpp"
#include "textured_button_renderer.hpp"
#include "ui/panel.hpp"

class PanelRenderer : public Renderer<Ui::Panel>
{
private:
    TextRenderer text_renderer;
    ButtonRenderer button_renderer;
    TexturedButtonRenderer textured_button_renderer;

public:
    PanelRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::Panel& ui, const float dt) override;
};
