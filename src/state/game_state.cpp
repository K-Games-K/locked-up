#include "game_state.hpp"

GameState::GameState(sf::RenderWindow& window, GameStateManager& game_state_manager)
    : window(window), game_state_manager(game_state_manager),
    textures("assets/sprites", "png"),
    fonts("assets/fonts", "ttf"),
    sound_buffers("assets/sounds", "wav")
{}

