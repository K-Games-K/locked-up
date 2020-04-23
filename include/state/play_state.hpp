#pragma once

#include "game_state.hpp"
#include "game_board.hpp"

#include <SFML/Graphics.hpp>

class PlayState : public GameState
{
private:
    const int GAMEBOARD_WIDTH = 100;
    const int GAMEBOARD_HEIGHT = 100;

    GameBoard board;

    sf::Texture backGroud;
    sf::Texture player;


public:
    PlayState(sf::RenderWindow& window);

    std::unique_ptr<GameState> handle_input(sf::Event event) override;

    std::unique_ptr<GameState> update(float dt) override;

    void render(float dt) override;
};
