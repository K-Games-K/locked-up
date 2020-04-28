#pragma once

#include "camera_renderer.hpp"
#include "game_board.hpp"

class GameBoardRenderer : public CameraRenderer<GameBoard>
{
private:
    const float TILE_SIZE = 60;

    sf::Sprite game_board_sprite;

public:
    GameBoardRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const GameBoard& game_board, const float dt) override;
};
