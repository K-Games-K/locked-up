#include <sstream>
#include <iomanip>

#include "state/lobby_state.hpp"
#include "state/play_state.hpp"
#include "state/main_menu_state.hpp"
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

    left_panel = new Ui::TexturedPanel(
        textures.get("paper"),
        {-40, 0},
        Ui::Anchor::CenterRight, Ui::Anchor::Center
    );
    user_interface.add_widget(left_panel);

    left_panel_title_text = new Ui::Text(
        "",
        font,
        {0, 40},
        {sf::Color::Black, 50},
        Ui::Anchor::CenterTop, Ui::Anchor::CenterTop
    );
    left_panel->add_widget(left_panel_title_text);

    left_panel->add_widget(
        new Ui::Button(
            "Not ready",
            font,
            std::bind(&LobbyState::ready_clicked, this, std::placeholders::_1),
            {0, -100},
            {420, 40},
            button_colors,
            {sf::Color::Red},
            Ui::Anchor::CenterBottom, Ui::Anchor::CenterBottom
        )
    );

    left_panel->add_widget(
        new Ui::Button(
            "Exit server",
            font,
            std::bind(&LobbyState::exit_clicked, this, std::placeholders::_1),
            {0, -40},
            {420, 40},
            button_colors,
            {sf::Color::Black},
            Ui::Anchor::CenterBottom, Ui::Anchor::CenterBottom
        )
    );

    players_list_text = new Ui::Text(
        "",
        font,
        {0, 150},
        {sf::Color::Black},
        Ui::Anchor::CenterTop, Ui::Anchor::CenterTop
    );
    left_panel->add_widget(players_list_text);

    right_panel = new Ui::TexturedPanel(
        textures.get("paper"),
        {40, 0},
        Ui::Anchor::CenterLeft, Ui::Anchor::Center
    );
    user_interface.add_widget(right_panel);

    right_panel->add_widget(
        new Ui::Text(
            "Chat coming soonTM...",
            font,
            {0, 0},
            {sf::Color::Black, 50},
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

    time -= dt;
    if(time < 0)
        time = 0;

    if(time > 0)
    {
        std::stringstream ss;
        ss << "Game starting in " << std::fixed << std::setprecision(1) << time << "s";
        left_panel_title_text->set_string(ss.str());
    }
    else
    {
        left_panel_title_text->set_string("Waiting for players...");
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
    std::cout << "[" << server_connection.get_addr() << "] sent a packet of id: "
        << packet->get_id() << std::endl;

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
        case GameBoardPacket::PACKET_ID:
        {
            auto game_board_packet = dynamic_cast<GameBoardPacket&>(*packet);
            game_board = game_board_packet.get_game_board();

            break;
        }
        case PlayerReadyPacket::PACKET_ID:
        {
            auto player_ready_packet = dynamic_cast<PlayerReadyPacket&>(*packet);
            uint16_t player_id = player_ready_packet.get_player_id();
            bool ready = player_ready_packet.is_ready();

            // TODO: Show which players are ready.

            break;
        }
        case CountdownPacket::PACKET_ID:
        {
            auto countdown_packet = dynamic_cast<CountdownPacket&>(*packet);
            time = countdown_packet.get_interval();

            break;
        }
        case GameStartPacket::PACKET_ID:
        {
            auto game_start_packet = dynamic_cast<GameStartPacket&>(*packet);
            sf::Vector2i start_pos(
                game_start_packet.get_start_x(), game_start_packet.get_start_y()
            );
            players_list.at(player_id).set_position(start_pos);

            auto& alibis = game_start_packet.get_alibis();
            for(auto& alibi : alibis)
            {
                for(int room : alibi)
                    std::cout << room << " ";
                std::cout << std::endl;
            }

            left_panel_title_text->set_string("Loading...");
            game_state_manager.push_state(
                new PlayState(
                    window, game_state_manager, server_connection, game_board,
                    player_id, players_list
                ), true
            );
        }
        default:
            break;
    }
}

void LobbyState::ready_clicked(Ui::Button& button)
{
    if(player_id == -1)
        return;

    ready = !ready;
    time = 0;
    button.get_text().set_string(ready ? "I'm ready!" : "Not ready");
    server_connection.send(PlayerReadyPacket(player_id, ready));
    button.get_text().set_color(ready ? sf::Color::Green : sf::Color::Red);
}

void LobbyState::exit_clicked(Ui::Button& button)
{
    server_connection.send(DisconnectPacket());
    game_state_manager.push_state(new MainMenuState(window, game_state_manager), true);
}
