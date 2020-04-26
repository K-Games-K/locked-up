#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "game_state_manager.hpp"

class GameState
{
protected:
    sf::RenderWindow& window;
    GameStateManager& game_state_manager;

public:
    GameState(sf::RenderWindow& window, GameStateManager& game_state_manager);

    virtual ~GameState() = default;

    /// Handles events provided by SFML.
    /// Can return new GameState to switch to.
    virtual void handle_input(sf::Event event) = 0;

    /// Updates game logic.
    /// Can return new GameState to switch to.
    virtual void update(float dt) = 0;

    /// Renders single frame.
    virtual void render(float dt) = 0;
};
