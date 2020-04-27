#pragma once

#include <thread>

#include "game_state.hpp"
#include "ui/widgets.hpp"
#include "render/panel_renderer.hpp"
#include "render/main_menu_background_renderer.hpp"

class MainMenuState : public GameState
{
private:
    const sf::Color CLEAR_COLOR = sf::Color(242, 0, 0);

    Ui::Panel user_interface;
    PanelRenderer panel_renderer;
    Ui::TextEdit* nickname_text_edit;
    Ui::TextEdit* address_text_edit;
    Ui::TextEdit* port_text_edit;
    Ui::Panel* join_game_panel;
    Ui::Text* connecting_text_widget;

    MainMenuBackgroundRenderer background_renderer;

    std::string nickname;
    Connection server_connection;
    std::thread connection_thread;

    void join_clicked(Ui::Button& button);

    void exit_clicked(Ui::Button& button);

    void connect_to_server(sf::IpAddress addr, unsigned short port);

public:
    MainMenuState(sf::RenderWindow& window, GameStateManager& game_state_manager);

    ~MainMenuState();

    void handle_input(sf::Event event) override;

    void update(float dt) override;

    void render(float dt) override;
};
