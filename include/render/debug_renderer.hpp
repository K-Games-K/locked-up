#pragma once

#include "player.hpp"
#include "game_board.hpp"
#include "renderer.hpp"

struct DebugContext
{
    int player_id;
    Player& player;
    GameBoard& game_board;
};

class DebugRenderer : public Renderer<DebugContext>
{
private:
    const float TILE_SIZE = 40;

    sf::Vector2f game_board_position;
    sf::Vector2f game_board_size;

public:
    DebugRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const DebugContext& context, sf::Vector2f camera_pos) override;

    void set_game_board_position(sf::Vector2f game_board_position);
    void set_game_board_size(sf::Vector2f game_board_size);
};
