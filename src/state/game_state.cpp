#include "state/game_state.hpp"

GameState::GameState(sf::RenderWindow& window, GameStateManager& game_state_manager)
        : window(window), game_state_manager(game_state_manager)
{}

