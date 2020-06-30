#pragma once

#include <vector>

#include "camera_renderer.hpp"
#include "text_renderer.hpp"
#include "../player.hpp"

class PlayerRenderer : public CameraRenderer<std::vector<Player>>
{
private:
    const float TILE_SIZE = 60;

    TextRenderer text_renderer;

    float speed = 2;
    float time = 0;

public:
    PlayerRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const std::vector<Player>& players, const float dt) override;
};
