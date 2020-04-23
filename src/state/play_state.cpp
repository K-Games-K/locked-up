#include <iostream>

#include "network/connection.hpp"
#include "network/packet/debug_packet.hpp"
#include "state/play_state.hpp"

PlayState::PlayState(sf::RenderWindow& window)
        : GameState(window), board(GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT)
{

}

std::unique_ptr<GameState> PlayState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
        window.close();

    return nullptr;
}

std::unique_ptr<GameState> PlayState::update(float dt)
{
    return nullptr;
}

void PlayState::render(float dt)
{

}
