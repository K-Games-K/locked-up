#pragma once

#include <SFML/Graphics.hpp>

#include "player.hpp"
#include "game_state.hpp"
#include "game_board.hpp"
#include "player.hpp"
#include "resource_manager.hpp"
#include "network/connection.hpp"

class PlayState : public GameState
{
private:
    const sf::IpAddress SERVER_ADDR = "127.0.0.1";
    const unsigned short SERVER_PORT = 2704;

    const sf::Color CLEAR_COLOR = sf::Color::Black;
    const int TILE_SIZE = 40;

    GameBoard game_board;
    sf::Vector2f camera_pos;

    int player_id;
    std::vector<Player> players;

    Connection server_connection;

    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;
    sf::Sprite player_sprite;
    sf::Sprite bg_sprite;

    bool debug_render = false;

    void packet_received(std::unique_ptr<Packet> packet);

public:
    PlayState(sf::RenderWindow& window);

    std::unique_ptr<GameState> handle_input(sf::Event event) override;

    std::unique_ptr<GameState> update(float dt) override;

    void render(float dt) override;
};
