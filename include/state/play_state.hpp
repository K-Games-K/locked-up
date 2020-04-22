#pragma once

#include <SFML/Window/Event.hpp>

#include "game_state.hpp"

class PlayState : public GameState
{
public:
    PlayState(sf::RenderWindow& window);

    std::unique_ptr<GameState> handle_input(sf::Event event) override;

    std::unique_ptr<GameState> update(float dt) override;

    void render(float dt) override;
};
