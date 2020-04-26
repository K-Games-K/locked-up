#pragma once

#include "renderer.hpp"
#include "text_renderer.hpp"
#include "button_renderer.hpp"
#include "ui/user_interface.hpp"

class UserInterfaceRenderer : public Renderer<Ui::UserInterface>
{
private:
    TextRenderer text_renderer;
    ButtonRenderer button_renderer;

public:
    UserInterfaceRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const Ui::UserInterface& ui, sf::Vector2f origin = {0, 0}) override;
};
