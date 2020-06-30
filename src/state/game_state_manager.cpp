#include "game_state_manager.hpp"
#include "game_state.hpp"

GameStateManager::~GameStateManager()
{
    while(!game_states.empty())
    {
        GameState* game_state = game_states.top();
        game_states.pop();
        delete game_state;
    }
    while(!queue.empty())
    {
        GameState* game_state = queue.front();
        queue.pop();
        delete game_state;
    }
}

void GameStateManager::update()
{
    if(pop_top)
    {
        GameState* game_state = game_states.top();
        game_states.pop();
        delete game_state;
        pop_top = false;
    }

    if(!queue.empty())
    {
        mutex.lock();

        GameState* game_state = queue.front();
        game_states.push(game_state);
        queue.pop();

        mutex.unlock();
    }
}

void GameStateManager::push_state(GameState* game_state, bool swap)
{
    mutex.lock();

    queue.push(game_state);
    pop_top = swap;

    mutex.unlock();
}

void GameStateManager::pop_state()
{
    pop_top = true;
}

GameState* GameStateManager::get_current_state()
{
    return !game_states.empty() ? game_states.top() : nullptr;
}

size_t GameStateManager::size() const
{
    return game_states.size();
}


