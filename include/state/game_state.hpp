#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GameState
{
protected:
    sf::RenderWindow& window;

public:
    GameState(sf::RenderWindow& window);

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
