#pragma once

#include <vector>

#include "player.hpp"
#include "camera_renderer.hpp"
#include "text_renderer.hpp"

class PlayerRenderer : public CameraRenderer<std::vector<Player>>
{
private:
    const float TILE_SIZE = 60;

    sf::Sprite player_sprite;
    TextRenderer text_renderer;

public:
    PlayerRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const std::vector<Player>& players, const float dt) override;
};
