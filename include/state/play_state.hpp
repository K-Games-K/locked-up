#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "player.hpp"
#include "game_state.hpp"
#include "game_board.hpp"
#include "player.hpp"
#include "resource_manager.hpp"
#include "network/connection.hpp"
#include "render/background_renderer.hpp"
#include "render/debug_renderer.hpp"
#include "render/player_renderer.hpp"
#include "render/game_board_renderer.hpp"
#include "render/panel_renderer.hpp"
#include "ui/widgets.hpp"

class PlayState : public GameState
{
private:
    const sf::Vector2f GAME_BOARD_SIZE = {900, 900};

    const sf::Color CLEAR_COLOR = sf::Color::Black;
    const float CAMERA_SMOOTH = 0.2;
    const int TILE_SIZE = 60;

    GameBoard game_board;

    sf::Sound walk_sound;
    sf::Sound paper_sound;
    sf::Vector2f camera_pos;
    sf::Vector2f window_viewport = {(float) window.getSize().x, (float) window.getSize().y};
    sf::Vector2f game_board_pos = {
        (window_viewport.y - GAME_BOARD_SIZE.y) / 2,
        (window_viewport.y - GAME_BOARD_SIZE.y) / 2
    };

    int player_id;
    int current_player_id = 0;
    std::vector<Player> players_list;

    Connection server_connection;

    Ui::Panel user_interface;
    Ui::Panel* action_panel;
    Ui::Text* current_room_text;
    Ui::Button* search_action_button;
    Ui::NotepadWidget* notepad_widget;
    Ui::NotificationWidget* notification_widget;
    Ui::Popup* popup;
    Ui::Panel* voting_menu;
    Ui::Panel* pause_menu;

    PlayerRenderer player_renderer;
    GameBoardRenderer game_board_renderer;
    DebugRenderer debug_renderer;
    PanelRenderer panel_renderer;
    BackgroundRenderer background_renderer;

    bool paused = false;
    bool debug_render = false;

    void packet_received(std::unique_ptr<Packet> packet);

    void resume_clicked(Ui::Button& button);

    void exit_clicked(Ui::Button& button);

    void action_clicked(Ui::Button& button);

    void vote_clicked(Ui::Button& button);

    sf::Vector2f window_to_board_coords(sf::Vector2f window_coords);

    sf::Vector2f board_to_window_coords(sf::Vector2f window_coords);

public:
    PlayState(sf::RenderWindow& window, GameStateManager& game_state_manager,
        Connection server_connection, const GameBoard& game_board, int player_id,
        const std::vector<Player>& players_list, const std::vector<std::vector<int>>& alibis,
        int crime_room, Item crime_item);

    void handle_input(sf::Event event) override;

    void update(float dt) override;

    void render(float dt) override;
};
