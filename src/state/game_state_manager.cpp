#include "state/game_state_manager.hpp"
#include "state/game_state.hpp"

GameStateManager::~GameStateManager()
{
    while(!game_states.empty())
    {
        GameState* game_state = game_states.top();
        game_states.pop();
        delete game_state;
    }
}

void GameStateManager::push_state(GameState* game_state)
{
    game_states.push(game_state);
}

void GameStateManager::swap_state(GameState* game_state)
{
    pop_state();
    game_states.push(game_state);
}

void GameStateManager::pop_state()
{
    GameState* old_state = game_states.top();
    to_delete.insert(old_state);
    game_states.pop();
}

GameState* GameStateManager::get_current_state()
{
    return !game_states.empty() ? game_states.top() : nullptr;
}

size_t GameStateManager::size() const
{
    return game_states.size();
}


