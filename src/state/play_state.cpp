#include <iostream>

#include "network/connection.hpp"
#include "network/packet/debug_packet.hpp"
#include "state/play_state.hpp"


PlayState::PlayState(sf::RenderWindow& window)
        : GameState(window), board(GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT)
{
    backGroud.loadFromFile("assets/sprites/board.png");

    player.loadFromFile("assets/sprites/player.png");

    players.push_back(Player("V_Ader", 10, 14));
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
        players[0].set_position(posx, posy);
    
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        players[0].set_position(players[0].get_x(), players[0].get_y() - 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        players[0].set_position(players[0].get_x(), players[0].get_y() + 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        players[0].set_position(players[0].get_x() - 1, players[0].get_y());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        players[0].set_position(players[0].get_x() + 1, players[0].get_y());
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

    sf::Sprite player_sprite;

    for (Player player_obj : players)
    {
        player_sprite.setTexture(player);

        player_sprite.setPosition(player_obj.get_x() * 40, player_obj.get_y() * 40);

        window.draw(player_sprite);
    }

}
