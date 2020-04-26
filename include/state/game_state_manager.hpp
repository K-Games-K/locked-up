#pragma once

#include <stack>
#include <unordered_set>
#include <SFML/Window/Event.hpp>

class GameState;

/// Stack-based manager for game states.
class GameStateManager
{
private:
    std::stack<GameState*> game_states;

    std::unordered_set<GameState*> to_delete;

public:
    ~GameStateManager();

    /// Pushes new game state on top of stack.
    void push_state(GameState* game_state);

    /// Puts new state on top of stack.
    /// Deletes old one.
    void swap_state(GameState* game_state);

    /// Pops and deletes state from top of stack.
    void pop_state();

    /// Returns state from top of stack (nullptr if stack is empty).
    GameState* get_current_state();

    /// Returns how many states are currently on stack.
    size_t size() const;
};
