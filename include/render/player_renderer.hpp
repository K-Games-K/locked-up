#pragma once

#include <vector>

#include "player.hpp"
#include "renderer.hpp"

class PlayerRenderer : public Renderer<std::vector<Player>>
{
private:
    const float TILE_SIZE = 40;

    sf::Sprite player_sprite;
    sf::Vector2f game_board_position;

public:
    PlayerRenderer(sf::RenderWindow& window, ResourceManagers resources);

    void render(const std::vector<Player>& players, sf::Vector2f camera_pos) override;

    void set_game_board_position(sf::Vector2f game_board_position);
};
