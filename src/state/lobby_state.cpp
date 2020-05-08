#include <iomanip>
#include <iostream>
#include <sstream>

#include "state/lobby_state.hpp"
#include "state/play_state.hpp"
#include "state/main_menu_state.hpp"
#include "network/packet/packets.hpp"

LobbyState::LobbyState(sf::RenderWindow& window, GameStateManager& game_state_manager,
    Connection&& server_connection, const std::string& nickname, const std::string& avatar_name)
    : GameState(window, game_state_manager), server_connection(std::move(server_connection)),
    master_widget_renderer(window),
    background_renderer(window, {textures, fonts})
{
    // Preload background texture.
    textures.get("map");

    auto& font = fonts.get("IndieFlower-Regular");
    auto base_button = Ui::Button()
        .set_default_color(Ui::Color(0, 0, 0, 140))
        .set_hover_color(Ui::Color(0, 0, 0, 190))
        .set_active_color(Ui::Color(0, 0, 0, 210));

    user_interface.set_size((sf::Vector2f) window.getSize());

    left_panel = (Ui::Panel*) user_interface.add_widget(
        Ui::Panel(textures.get("paper"))
            .set_position({-40, 0})
            .set_origin(Ui::Origin::CenterRight)
    );

    left_panel_title_text = (Ui::Text*) left_panel->add_widget(
        Ui::Text(font)
            .set_font_size(50)
            .set_position({0, 40})
            .set_origin(Ui::Origin::CenterTop)
            .set_anchor(Ui::Anchor::CenterTop)
    );

    auto ready_button = left_panel->add_widget(
        base_button
            .set_callback(std::bind(&LobbyState::ready_clicked, this, std::placeholders::_1))
            .set_position({0, -100})
            .set_size({420, 40})
            .set_origin(Ui::Origin::CenterBottom)
            .set_anchor(Ui::Anchor::CenterBottom)
    );
    ready_button->add_widget(
        Ui::Text(font, "Not ready")
            .set_color(Ui::Color::Red)
    );

    auto exit_button = left_panel->add_widget(
        base_button
            .set_callback(std::bind(&LobbyState::exit_clicked, this, std::placeholders::_1))
            .set_position({0, -40})
            .set_size({420, 40})
    );
    exit_button->add_widget(Ui::Text(font, "Exit server"));

    players_list_layout = (Ui::Layout*) left_panel->add_widget(
        Ui::Layout(Ui::LayoutType::Vertical, 5)
    );

    right_panel = (Ui::Panel*) user_interface.add_widget(
        Ui::Panel(textures.get("paper"))
            .set_position({40, 0})
            .set_origin(Ui::Origin::CenterLeft)
            .set_anchor(Ui::Anchor::Center)
    );
    right_panel->add_widget(
        Ui::Text(font, "Chat coming soonTM...")
            .set_font_size(50)
    );

    this->server_connection.send(JoinGamePacket(nickname, avatar_name));
}

void LobbyState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
    {
        server_connection.send(DisconnectPacket());
        game_state_manager.pop_state();
    }

    user_interface.handle_event(
        event, (sf::Vector2f) sf::Mouse::getPosition(window),
        {0, 0}, (sf::Vector2f) window.getSize()
    );
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

    background_renderer.render(textures.get("map"), dt);

    master_widget_renderer.render(user_interface, dt, {0, 0}, (sf::Vector2f) window.getSize());
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

            players_list_layout->clear();
            for(auto& player : players_list)
            {
                auto layout = (Ui::Layout*) players_list_layout->add_widget(
                    Ui::Layout(Ui::LayoutType::Horizontal, 10)
                );
                layout->add_widget(
                    Ui::Panel(textures.get(player.get_avatar_name()))
                        .set_size({33, 50})
                );
                layout->add_widget(
                    Ui::Text(fonts.get("IndieFlower-Regular"), player.get_nickname())
                );
                players_list_layout->update_size();
            }

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
            int crime_room = game_start_packet.get_crime_room();
            Item crime_item = game_start_packet.get_crime_item();

            left_panel_title_text->set_string("Loading...");
            game_state_manager.push_state(
                new PlayState(
                    window, game_state_manager, std::move(server_connection), game_board,
                    player_id, players_list, alibis, crime_room, crime_item
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
    button.get_child<Ui::Text>()->set_string(ready ? "I'm ready!" : "Not ready");
    server_connection.send(PlayerReadyPacket(player_id, ready));
    button.get_child<Ui::Text>()->set_color(ready ? Ui::Color::Green : Ui::Color::Red);
}

void LobbyState::exit_clicked(Ui::Button& button)
{
    server_connection.send(DisconnectPacket());
    game_state_manager.push_state(new MainMenuState(window, game_state_manager), true);
}
