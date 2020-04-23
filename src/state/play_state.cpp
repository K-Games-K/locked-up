#include <iostream>

#include "network/connection.hpp"
#include "network/packet/debug_packet.hpp"
#include "state/play_state.hpp"

#include "player.hpp"

PlayState::PlayState(sf::RenderWindow& window)
        : GameState(window), board(GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT)
{
    backGroud.loadFromFile("assets/sprites/board.png");

    player.loadFromFile("assets/sprites/player.png");
}

std::unique_ptr<GameState> PlayState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
        window.close();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {

        sf::Vector2i mosepos = sf::Mouse::getPosition(this->window);

        int posx = (int)(mosepos.x / 40);
        int posy = (int)(mosepos.y / 40);

        std::cout << "room: " << board.get_room(posx, posy).get_name() << std::endl;
        printf("mose at: %d %d\n", posx, posy);
    
    }

    return nullptr;
}

std::unique_ptr<GameState> PlayState::update(float dt)
{
    return nullptr;
}

void PlayState::render(float dt)
{
    sf::Sprite temp;
    temp.setTexture(backGroud);

    window.draw(temp);

    sf::Sprite temp2;

    temp2.setTexture(player);

    temp2.setPosition(400, 400);

    window.draw(temp2);


}
