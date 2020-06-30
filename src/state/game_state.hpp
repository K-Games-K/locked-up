#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game_state_manager.hpp"
#include "../resource_manager.hpp"

class GameState
{
protected:
    sf::RenderWindow& window;
    GameStateManager& game_state_manager;

    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;
    ResourceManager<sf::SoundBuffer> sound_buffers;

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
