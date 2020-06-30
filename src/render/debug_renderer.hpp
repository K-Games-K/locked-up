#pragma once

#include "camera_renderer.hpp"
#include "../player.hpp"
#include "../game_board.hpp"

struct DebugContext
{
    int player_id;
    Player& player;
    GameBoard& game_board;
};

class DebugRenderer : public CameraRenderer<DebugContext>
{
private:
    const float TILE_SIZE = 60;

public:
    DebugRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const DebugContext& context, const float dt) override;
};
