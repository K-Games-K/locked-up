#include <sstream>

#include "state/lobby_state.hpp"
#include "network/packet/packets.hpp"

LobbyState::LobbyState(sf::RenderWindow& window, GameStateManager& game_state_manager,
    Connection server_connection)
    : GameState(window, game_state_manager), server_connection(server_connection),
    user_interface({0, 0}, (sf::Vector2f) window.getSize()),
    panel_renderer(window, {textures, fonts}),
    background_renderer(window, {textures, fonts})
{
    // Preload background texture.
    textures.get("mapa4");
    sf::Font& font = fonts.get("IndieFlower-Regular");
    Ui::ButtonColors button_colors = {
        sf::Color(0, 0, 0, 140),
        sf::Color(0, 0, 0, 190),
        sf::Color(0, 0, 0, 210)
    };

    left_panel = new Ui::Panel(
        {-40, 0}, {500, 700},
        sf::Color(242, 0, 0, 150),
        Ui::Anchor::CenterRight, Ui::Anchor::Center
    );
    user_interface.add_widget(left_panel);

    left_panel->add_widget(
        new Ui::Text(
            "Waiting for players...",
            font,
            {0, 30},
            {sf::Color::White, 50, sf::Color::Black, 0},
            Ui::Anchor::CenterTop, Ui::Anchor::CenterTop
        )
    );

    left_panel->add_widget(
        new Ui::Button(
            "Not ready",
            font,
            std::bind(&LobbyState::ready_clicked, this, std::placeholders::_1),
            {0, -50},
            {420, 40},
            button_colors,
            Ui::TextSettings(),
            Ui::Anchor::CenterBottom,
            Ui::Anchor::CenterBottom
        )
    );

    players_list_text = new Ui::Text(
        "",
        font,
        {0, 150},
        Ui::TextSettings(),
        Ui::Anchor::CenterTop, Ui::Anchor::CenterTop
    );
    left_panel->add_widget(players_list_text);

    right_panel = new Ui::Panel(
        {40, 0}, {500, 700},
        sf::Color(242, 0, 0, 150),
        Ui::Anchor::CenterLeft, Ui::Anchor::Center
    );
    user_interface.add_widget(right_panel);

    right_panel->add_widget(
        new Ui::Text(
            "Chat coming soonTM...",
            font,
            {0, 0},
            {sf::Color::White, 50, sf::Color::Black, 0},
            Ui::Anchor::Center, Ui::Anchor::Center
        )
    );
}

void LobbyState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
    {
        server_connection.send(DisconnectPacket());
        game_state_manager.pop_state();
    }

    sf::Vector2f mouse_pos =
        (sf::Vector2f) sf::Mouse::getPosition(window) - user_interface.get_relative_position(
            {0, 0},
            (sf::Vector2f) window.getSize()
        );
    user_interface.handle_event(event, mouse_pos);
}

void LobbyState::update(float dt)
{
    if(server_connection.is_connected())
    {
        auto packet = server_connection.recv();
        if(packet != nullptr)
            packet_received(std::move(packet));
    }
}

void LobbyState::render(float dt)
{
    window.clear(CLEAR_COLOR);

    background_renderer.render(textures.get("mapa4"), dt);

    panel_renderer.render(user_interface, dt);
}

void LobbyState::packet_received(std::unique_ptr<Packet> packet)
{
    switch(packet->get_id())
    {
        case PlayersListPacket::PACKET_ID:
        {
            auto players_list_packet = dynamic_cast<PlayersListPacket&>(*packet);
            player_id = players_list_packet.get_player_id();
            players_list = players_list_packet.get_players_list();

            std::stringstream ss;
            for(auto& player : players_list)
                ss << player.get_nickname() << std::endl;
            std::string string = ss.str();
            string.pop_back();
            players_list_text->set_string(string);

            break;
        }
        default:
            break;
    }
}

void LobbyState::ready_clicked(Ui::Button& button)
{
    ready = !ready;
    button.get_text().set_string(ready ? "I'm ready!" : "Not ready");
    button.get_text().set_color(ready ? sf::Color::Green : sf::Color::White);
}
