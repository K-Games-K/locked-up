#include <functional>
#include <iostream>

#include "utils.hpp"
#include "network/connection.hpp"
#include "state/main_menu_state.hpp"
#include "state/lobby_state.hpp"
#include "network/packet/join_game_packet.hpp"

MainMenuState::MainMenuState(sf::RenderWindow& window, GameStateManager& game_state_manager)
    : GameState(window, game_state_manager),
    user_interface(
        textures.get("paper"),
        {0, 0},
        Ui::Anchor::Center,
        Ui::Anchor::Center
    ),
    panel_renderer(window, {textures, fonts}),
    background_renderer(window, {textures, fonts})
{
    panel_renderer.set_origin_pos({0, 0});
    panel_renderer.set_parent_size((sf::Vector2f) window.getSize());

    // Preload background texture.
    textures.get("map");

    sf::Font& font = fonts.get("IndieFlower-Regular");
    user_interface.add_widget(
        new Ui::Text(
            "Locked Up!",
            font,
            {0, 30},
            {sf::Color::Black, 95},
            Ui::Anchor::CenterTop, Ui::Anchor::CenterTop
        )
    );

    join_game_panel = new Ui::Panel(
        {0, 0}, {420, 100},
        sf::Color::Transparent,
        Ui::Anchor::CenterBottom,
        Ui::Anchor::Center
    );
    join_game_panel->add_widget(
        new Ui::Text(
            "Nickname:",
            font,
            {-210, -80},
            {sf::Color::Black},
            Ui::Anchor::CenterLeft, Ui::Anchor::Center
        )
    );
    nickname_text_edit = new Ui::TextEdit(
        font,
        {0, -40}, {420, 40},
        Ui::TextEditColors(),
        24,
        Ui::Anchor::Center, Ui::Anchor::Center
    );
    join_game_panel->add_widget(nickname_text_edit);
    join_game_panel->add_widget(
        new Ui::Text(
            "Server address:",
            font,
            {-210, 0},
            {sf::Color::Black},
            Ui::Anchor::CenterLeft, Ui::Anchor::Center
        )
    );
    address_text_edit = new Ui::TextEdit(
        font,
        {90, 40}, {300, 40},
        Ui::TextEditColors(),
        17,
        Ui::Anchor::CenterRight, Ui::Anchor::Center
    );
    join_game_panel->add_widget(address_text_edit);
    join_game_panel->add_widget(
        new Ui::Text(
            "Port:",
            font,
            {110, 0},
            {sf::Color::Black},
            Ui::Anchor::CenterLeft, Ui::Anchor::Center
        )
    );
    port_text_edit = new Ui::TextEdit(
        font,
        {110, 40}, {100, 40},
        Ui::TextEditColors(),
        5,
        Ui::Anchor::CenterLeft, Ui::Anchor::Center
    );
    join_game_panel->add_widget(port_text_edit);
    user_interface.add_widget(join_game_panel);

    Ui::ButtonColors button_colors = {
        sf::Color(0, 0, 0, 140),
        sf::Color(0, 0, 0, 190),
        sf::Color(0, 0, 0, 210)
    };

    join_game_panel->add_widget(
        new Ui::Button(
            "Join the game",
            font,
            std::bind(&MainMenuState::join_clicked, this, std::placeholders::_1),
            {0, 100}, {420, 40},
            button_colors,
            {sf::Color::Black},
            Ui::Anchor::Center,
            Ui::Anchor::Center
        )
    );
    avatars_table = new Ui::TableWidget(
        3, 2,
        { 100, 100, 100 },
        { 80, 80 },
        { 0,210 },
        {},
        Ui::Anchor::Center,
        Ui::Anchor::Center
    );
    avatars_table->set_grid_thickness(0.001);
    std::vector<std::string> avatar_name = {"Mr3", "Mr2","Mr1","Mrs3","Mrs2","Mrs1" };
    for (int i = 0; i <= 1; ++i)
    {
        for (int j = 0; j <= 2; ++j)
        {
            avatars_table->add_widget(j, i,
                new Ui::Button(
                    avatar_name.back(),
                    font,
                    std::bind(&MainMenuState::join_clicked, this, std::placeholders::_1),
                    { 0, 0 }, { 80, 80 },
                    button_colors,
                    { sf::Color::Black },
                    Ui::Anchor::Center,
                    Ui::Anchor::Center
                )
            );
            avatar_name.pop_back();
        }
    }
    join_game_panel->add_widget(avatars_table);

    user_interface.add_widget(
        new Ui::Button(
            "Exit",
            font,
            std::bind(&MainMenuState::exit_clicked, this, std::placeholders::_1),
            {0, -50},
            {420, 40},
            button_colors,
            {sf::Color::Black},
            Ui::Anchor::CenterBottom,
            Ui::Anchor::CenterBottom
        )
    );

    connecting_text_widget = new Ui::Text(
        "Connecting to the server...",
        font,
        {0, 0},
        {sf::Color::Black},
        Ui::Anchor::Center,
        Ui::Anchor::Center
    );
    connecting_text_widget->set_enabled(false);
    user_interface.add_widget(connecting_text_widget);

    // For easier debug fill login menu
    nickname_text_edit->get_text().set_string("Debugger");
    address_text_edit->get_text().set_string("localhost");
    port_text_edit->get_text().set_string("2704");
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

    sf::Vector2f mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(window);
    sf::Vector2f mouse_pos_rel = mouse_pos - user_interface.get_relative_position(
        {0, 0},
        (sf::Vector2f) window.getSize()
    );
    user_interface.handle_event(event, mouse_pos_rel);
}

void MainMenuState::update(float dt)
{
    if(server_connection.is_connected())
    {
        server_connection.send(JoinGamePacket(nickname));
        game_state_manager.push_state(
            new LobbyState(window, game_state_manager, server_connection),
            true
        );
    }
}

void MainMenuState::render(float dt)
{
    window.clear(CLEAR_COLOR);

    background_renderer.render(textures.get("map"), dt);

    panel_renderer.render(user_interface, dt);
}

void MainMenuState::join_clicked(Ui::Button& button)
{
    nickname = nickname_text_edit->get_text().get_string();
    std::string addr_str = address_text_edit->get_text().get_string();
    std::string port_str = port_text_edit->get_text().get_string();
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
