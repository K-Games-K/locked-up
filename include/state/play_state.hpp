#pragma once

#include "player.hpp"
#include "game_state.hpp"
#include "game_board.hpp"
#include "network/connection.hpp"

class PlayState : public GameState
{
private:
    const int GAMEBOARD_WIDTH = 100;
    const int GAMEBOARD_HEIGHT = 100;
    const sf::IpAddress SERVER_ADDR = "127.0.0.1";
    const unsigned short SERVER_PORT = 2704;

    GameBoard board;
    std::vector<Player> players;

    Connection server_connection;

    void packet_received(std::unique_ptr<Packet> packet);

public:
    PlayState(sf::RenderWindow& window);

    std::unique_ptr<GameState> handle_input(sf::Event event) override;

    std::unique_ptr<GameState> update(float dt) override;

    void render(float dt) override;
};
