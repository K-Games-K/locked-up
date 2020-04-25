#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

#include "state/game_state.hpp"

class Game
{
private:
    const std::string WINDOW_NAME = "Locked Out! [InDev]";
    const unsigned int WINDOW_WIDTH = 1300;
    const unsigned int WINDOW_HEIGHT = 900;

    std::unique_ptr<GameState> current_state;

    sf::RenderWindow window;

public:
    Game();

    void run();
};
