#pragma once

#include "renderer.hpp"

/// Base class for renderers of objects that render based on camera position.
template<typename T>
class CameraRenderer : public Renderer<T>
{
protected:
    sf::Vector2f camera_pos;
    sf::Vector2f game_board_pos;
    sf::Vector2f game_board_size;

public:
    CameraRenderer(sf::RenderWindow& window, ResourceManagers resources)
        : Renderer<T>(window, resources)
    {}

    void set_camera_pos(sf::Vector2f camera_pos)
    {
        this->camera_pos = camera_pos;
    }

    void set_game_board_pos(sf::Vector2f game_board_pos)
    {
        this->game_board_pos = game_board_pos;
    }

    void set_game_board_size(sf::Vector2f game_board_size)
    {
        this->game_board_size = game_board_size;
    }
};
