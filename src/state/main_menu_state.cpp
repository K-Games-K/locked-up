#include <functional>
#include <iostream>

#include "main_menu_state.hpp"
#include "lobby_state.hpp"
#include "../utils.hpp"
#include "../network/connection.hpp"
#include "../network/packet/join_game_packet.hpp"

MainMenuState::MainMenuState(sf::RenderWindow& window, GameStateManager& game_state_manager)
    : GameState(window, game_state_manager),
    user_interface(textures.get("paper")),
    master_widget_renderer(window),
    background_renderer(window, {textures, fonts})
{
    // Preload background texture.
    textures.get("map");

    click_sound.setBuffer(sound_buffers.get("click_sound"));
    click_sound.setVolume(20);

    auto& font = fonts.get("IndieFlower-Regular");
    auto default_color = Ui::Color(0, 0, 0, 140);
    auto hover_color = Ui::Color(0, 0, 0, 190);
    auto active_color = Ui::Color(0, 0, 0, 210);

    user_interface.add_widget(
        Ui::Text(font, "Locked Up!")
            .set_font_size(95)
            .set_position({0, 30})
            .set_origin(Ui::Origin::CenterTop)
            .set_anchor(Ui::Anchor::CenterTop)
    );

    join_game_panel = (Ui::Panel*) user_interface.add_widget(
        Ui::Panel()
            .set_size({420, 100})
            .set_origin(Ui::Origin::CenterBottom)
    );

    join_game_panel->add_widget(
        Ui::Text(font, "Nickname:")
            .set_position({-210, -130})
            .set_origin(Ui::Origin::CenterLeft)
    );
    nickname_line_edit = (Ui::LineEdit*) join_game_panel->add_widget(
        Ui::LineEdit(font)
            .set_position({0, -90})
            .set_size({420, 40})
    );

    join_game_panel->add_widget(
        Ui::Text(font, "Server address:")
            .set_position({-210, -50})
            .set_origin(Ui::Origin::CenterLeft)
    );
    address_line_edit = (Ui::LineEdit*) join_game_panel->add_widget(
        Ui::LineEdit(font)
            .set_max_length(17)
            .set_position({90, -10})
            .set_size({300, 40})
            .set_origin(Ui::Origin::CenterRight)
    );

    join_game_panel->add_widget(
        Ui::Text(font, "Port:")
            .set_position({110, -50})
            .set_origin(Ui::Origin::CenterLeft)
    );
    port_line_edit = (Ui::LineEdit*) join_game_panel->add_widget(
        Ui::LineEdit(font)
            .set_max_length(5)
            .set_position({110, -10})
            .set_size({100, 40})
            .set_origin(Ui::Origin::CenterLeft)
    );

    auto join_button = join_game_panel->add_widget(
        Ui::Button()
            .set_callback(std::bind(&MainMenuState::join_clicked, this, std::placeholders::_1))
            .set_default_color(default_color)
            .set_hover_color(hover_color)
            .set_active_color(active_color)
            .set_position({0, 50})
            .set_size({420, 40})
    );
    join_button->add_widget(Ui::Text(font, "Join the game"));

    auto avatars_table = (Ui::TableWidget*) join_game_panel->add_widget(
        Ui::TableWidget(3, 3)
            .set_column_widths({80, 80, 80})
            .set_row_heights({80, 80, 80})
            .set_grid_thickness(0)
            .set_position({0, 210})
    );

    for(int row = 0; row < 3; ++row)
    {
        for(int column = 0; column < 3; ++column)
        {
            auto avatar_name = avatars[column + row * 3];
            auto button = avatars_table->add_widget(
                column, row, Ui::Button()
                    .set_callback(
                        [this, avatar_name](Ui::Button& button) { avatar_clicked(avatar_name); }
                    )
                    .set_default_color(default_color)
                    .set_hover_color(hover_color)
                    .set_active_color(active_color)
                    .set_size({80, 80})
            );
            button->add_widget(Ui::Panel(textures.get(avatar_name)));
        }
    }

    auto exit_button = user_interface.add_widget(
        Ui::Button()
            .set_callback(std::bind(&MainMenuState::exit_clicked, this, std::placeholders::_1))
            .set_default_color(default_color)
            .set_hover_color(hover_color)
            .set_active_color(active_color)
            .set_position({0, -20})
            .set_size({420, 40})
            .set_origin(Ui::Origin::CenterBottom)
            .set_anchor(Ui::Anchor::CenterBottom)
    );
    exit_button->add_widget(Ui::Text(font, "Exit"));

    connecting_text_widget = (Ui::Text*) user_interface.add_widget(
        Ui::Text(font, "Connecting to the server...")
            .set_enabled(false)
    );

    // For easier debug fill login menu
    nickname_line_edit->set_text("Debugger");
    address_line_edit->set_text("localhost");
    port_line_edit->set_text("2704");
}

MainMenuState::~MainMenuState()
{
    if(connection_thread.joinable())
        connection_thread.join();
}

void MainMenuState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
        game_state_manager.pop_state();

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        click_sound.play();

    user_interface.handle_event(
        event, (sf::Vector2f) sf::Mouse::getPosition(window),
        {0, 0}, (sf::Vector2f) window.getSize()
    );
}

void MainMenuState::update(float dt)
{
    if(server_connection.is_connected())
    {
        game_state_manager.push_state(
            new LobbyState(
                window, game_state_manager, std::move(server_connection), nickname, avatar_name
            ),
            true
        );
    }
}

void MainMenuState::render(float dt)
{
    window.clear(CLEAR_COLOR);

    background_renderer.render(textures.get("map"), dt);

    master_widget_renderer.render(user_interface, dt, {0, 0}, (sf::Vector2f) window.getSize());
}

void MainMenuState::join_clicked(Ui::Button& button)
{
    nickname = nickname_line_edit->get_text().get_string();
    std::string addr_str = address_line_edit->get_text().get_string();
    std::string port_str = port_line_edit->get_text().get_string();
    if(addr_str.empty() || port_str.empty() || nickname.empty() ||
        !Utils::is_printable(nickname) || !Utils::is_number(port_str))
        return;

    unsigned short port = std::stoi(port_str);
    if(port > 65535)
        return;

    join_game_panel->set_enabled(false);
    connecting_text_widget->set_enabled(true);

    if(connection_thread.joinable())
        connection_thread.join();
    connection_thread = std::thread(&MainMenuState::connect_to_server, this, addr_str, port);
}

void MainMenuState::exit_clicked(Ui::Button& button)
{
    game_state_manager.pop_state();
}

void MainMenuState::connect_to_server(sf::IpAddress addr, unsigned short port)
{
    server_connection = Connection(addr, port);
    if(!server_connection.is_connected())
    {
        connecting_text_widget->set_enabled(false);
        join_game_panel->set_enabled(true);
    }
    else
    {
        connecting_text_widget->set_string("Loading...");
    }
}

void MainMenuState::avatar_clicked(std::string avatar_name)
{
    this->avatar_name = avatar_name;
}
