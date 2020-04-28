#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

#include "state/game_state_manager.hpp"

class Game
{
private:
    const std::string WINDOW_NAME = "Locked Out! [Alpha]";
    const unsigned int WINDOW_WIDTH = 1920;
    const unsigned int WINDOW_HEIGHT = 1080;

    GameStateManager game_state_manager;

    sf::RenderWindow window;

public:
    Game();

    void run();
};
