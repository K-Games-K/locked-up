#include <iostream>

#include "state/play_state.hpp"

PlayState::PlayState(sf::RenderWindow& window)
        : GameState(window)
{
    std::cout << "Constructor!" << std::endl;
}

PlayState::~PlayState()
{
    std::cout << "Destructor!" << std::endl;
}

std::unique_ptr<GameState> PlayState::handle_input(sf::Event event)
{
    return nullptr;
}

std::unique_ptr<GameState> PlayState::update(float dt)
{
    return nullptr;
}

void PlayState::render(float dt)
{

}
