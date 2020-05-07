#pragma once

#include <vector>

#include "player.hpp"
#include "game_board.hpp"
#include "game_state.hpp"
#include "network/connection.hpp"
#include "ui/widgets.hpp"
#include "render/master_widget_renderer.hpp"
#include "render/animated_background_renderer.hpp"

class LobbyState : public GameState
{
private:
    const sf::Color CLEAR_COLOR = sf::Color::Black;

    Connection server_connection;

    Ui::Panel user_interface;
    Ui::Panel* left_panel;
    Ui::Panel* right_panel;
    Ui::Text* left_panel_title_text;
    Ui::Layout* players_list_layout;

    MasterWidgetRenderer master_widget_renderer;
    AnimatedBackgroundRenderer background_renderer;

    int player_id = -1;
    std::vector<Player> players_list;
    GameBoard game_board;

    bool ready = false;
    float time = 0;

    void packet_received(std::unique_ptr<Packet> packet);

    void ready_clicked(Ui::Button& button);

    void exit_clicked(Ui::Button& button);

public:
    LobbyState(sf::RenderWindow& window, GameStateManager& game_state_manager,
        Connection&& server_connection, const std::string& nickname,
        const std::string& avatar_name);

    void handle_input(sf::Event event) override;

    void update(float dt) override;

    void render(float dt) override;
};
