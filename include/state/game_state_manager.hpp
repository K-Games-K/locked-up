#pragma once

#include <stack>
#include <queue>
#include <mutex>
#include <SFML/Window/Event.hpp>

class GameState;

/// Stack-based manager for game states.
class GameStateManager
{
private:
    std::stack<GameState*> game_states;

    std::mutex mutex;
    std::queue<GameState*> queue;
    bool pop_top = false;

public:
    ~GameStateManager();

    void update();

    /// Pushes new game state on top of stack.
    /// If swap is true pops current state from stack.
    void push_state(GameState* game_state, bool swap = false);

    /// Pops and deletes state from top of stack.
    void pop_state();

    /// Returns state from top of stack (nullptr if stack is empty).
    GameState* get_current_state();

    /// Returns how many states are currently on stack.
    size_t size() const;
};
