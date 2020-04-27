#pragma once

#include <vector>

#include "player.hpp"
#include "game_state.hpp"
#include "network/connection.hpp"
#include "ui/widgets.hpp"
#include "render/panel_renderer.hpp"
#include "render/animated_background_renderer.hpp"

class LobbyState : public GameState
{
private:
    const sf::Color CLEAR_COLOR = sf::Color::Black;

    Connection server_connection;

    Ui::Panel user_interface;
    PanelRenderer panel_renderer;
    Ui::TexturedPanel* left_panel;
    Ui::TexturedPanel* right_panel;
    Ui::Text* players_list_text;

    AnimatedBackgroundRenderer background_renderer;

    int player_id = -1;
    std::vector<Player> players_list;

    bool ready = false;

    void packet_received(std::unique_ptr<Packet> packet);

    void ready_clicked(Ui::Button& button);

public:
    LobbyState(sf::RenderWindow& window, GameStateManager& game_state_manager,
        Connection server_connection);

    void handle_input(sf::Event event) override;

    void update(float dt) override;

    void render(float dt) override;
};
