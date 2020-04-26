#pragma once

#include <SFML/Graphics.hpp>

#include "player.hpp"
#include "game_state.hpp"
#include "game_board.hpp"
#include "player.hpp"
#include "resource_manager.hpp"
#include "network/connection.hpp"
#include "render/debug_renderer.hpp"
#include "render/player_renderer.hpp"
#include "render/game_board_renderer.hpp"
#include "render/panel_renderer.hpp"
#include "ui/panel.hpp"

class PlayState : public GameState
{
private:
    const sf::IpAddress SERVER_ADDR = "127.0.0.1";
    const unsigned short SERVER_PORT = 2704;

    const sf::Vector2f GAME_BOARD_SIZE = {768, 768};

    const sf::Color CLEAR_COLOR = sf::Color::Black;
    const int TILE_SIZE = 40;

    GameBoard game_board;

    sf::Vector2f camera_pos;
    sf::Vector2f window_viewport = {(float)window.getSize().x, (float)window.getSize().y};
    sf::Vector2f game_board_pos = {
            (window_viewport.y - GAME_BOARD_SIZE.y) / 2,
            (window_viewport.y - GAME_BOARD_SIZE.y) / 2
    };

    int player_id;
    std::vector<Player> players;

    Connection server_connection;

    Ui::Panel user_interface;

    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;

    PlayerRenderer player_renderer;
    GameBoardRenderer game_board_renderer;
    DebugRenderer debug_renderer;
    PanelRenderer user_interface_renderer;

    bool debug_render = false;

    void packet_received(std::unique_ptr<Packet> packet);

    sf::Vector2f window_to_board_coords(sf::Vector2f window_coords);

    sf::Vector2f board_to_window_coords(sf::Vector2f window_coords);

public:
    PlayState(sf::RenderWindow& window);

    std::unique_ptr<GameState> handle_input(sf::Event event) override;

    std::unique_ptr<GameState> update(float dt) override;

    void render(float dt) override;
};
