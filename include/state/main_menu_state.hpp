#pragma once

#include <thread>

#include "game_state.hpp"
#include "ui/widgets.hpp"
#include "render/master_widget_renderer.hpp"
#include "render/animated_background_renderer.hpp"

class MainMenuState : public GameState
{
private:
    const sf::Color CLEAR_COLOR = sf::Color(242, 0, 0);
    const std::vector<std::string> avatars = {
        "mrs1", "mrs2", "mrs3",
        "mr1", "mr2", "mr3"
    };

    Ui::TexturedPanel user_interface;
    Ui::LineEdit* nickname_line_edit;
    Ui::LineEdit* address_line_edit;
    Ui::LineEdit* port_line_edit;
    Ui::Panel* join_game_panel;
    Ui::Text* connecting_text_widget;

    MasterWidgetRenderer master_widget_renderer;
    AnimatedBackgroundRenderer background_renderer;

    std::string nickname;
    std::string avatar = "mr1";
    Connection server_connection;
    std::thread connection_thread;

    void join_clicked(Ui::Button& button);

    void exit_clicked(Ui::Button& button);

    void avatar_clicked(std::string avatar_name);

    void connect_to_server(sf::IpAddress addr, unsigned short port);

public:
    MainMenuState(sf::RenderWindow& window, GameStateManager& game_state_manager);

    ~MainMenuState();

    void handle_input(sf::Event event) override;

    void update(float dt) override;

    void render(float dt) override;
};
