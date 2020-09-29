#include <iomanip>
#include <iostream>
#include <sstream>

#include "logging.hpp"
#include "game_board_loader.hpp"
#include "state/lobby_state.hpp"
#include "state/play_state.hpp"
#include "state/main_menu_state.hpp"

LobbyState::LobbyState(sf::RenderWindow& window, GameStateManager& game_state_manager,
    Connection&& server_connection, const std::string& nickname, const std::string& avatar_name)
    : GameState(window, game_state_manager), server_connection(std::move(server_connection)),
    master_widget_renderer(window),
    background_renderer(window, {textures, fonts})
{
    // Preload background texture.
    textures.get("map");

    click_sound.setBuffer(sound_buffers.get("click_sound"));
    click_sound.setVolume(20);

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
            .set_callback([this] (Ui::Button& button) {ready_clicked(button);})
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
            .set_callback([this] (Ui::Button& button) {exit_clicked(button);})
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

    Packet::JoinGamePacket join_game_packet;
    join_game_packet.set_nickname(nickname);
    join_game_packet.set_avatar_name(avatar_name);
    this->server_connection.send(join_game_packet);
}

void LobbyState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
    {
        server_connection.send(Packet::DisconnectPacket());
        game_state_manager.pop_state();
    }

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        click_sound.play();

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
        if(packet.has_value())
            packet_received(*packet);
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

void LobbyState::packet_received(const Packet::Any& packet)
{
    if(packet.Is<Packet::PlayersListPacket>())
    {
        Packet::PlayersListPacket players_list_packet;
        packet.UnpackTo(&players_list_packet);

        player_id = players_list_packet.player_id();
        players_list.clear();
        for(auto& entry : players_list_packet.players())
        {
            players_list.emplace_back(entry.nickname(), entry.avatar_name());
            players_list[players_list.size() - 1].set_position(entry.position_x(), entry.position_y());
        }

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
            // players_list_layout->update_size();
        }
    }
    else if(packet.Is<Packet::GameBoardPacket>())
    {
        Packet::GameBoardPacket game_board_packet;
        packet.UnpackTo(&game_board_packet);

        GameBoardLoader::load_from_packet(game_board, game_board_packet);
    }
    else if(packet.Is<Packet::PlayerReadyPacket>())
    {
        Packet::PlayerReadyPacket player_ready_packet;
        packet.UnpackTo(&player_ready_packet);

        uint16_t player_id = player_ready_packet.player_id();
        bool ready = player_ready_packet.ready();

        // TODO: Show which players are ready.
    }
    else if(packet.Is<Packet::CountdownPacket>())
    {
        Packet::CountdownPacket countdown_packet;
        packet.UnpackTo(&countdown_packet);
        time = countdown_packet.interval();
    }
    else if(packet.Is<Packet::GameStartPacket>())
    {
        Packet::GameStartPacket game_start_packet;
        packet.UnpackTo(&game_start_packet);

        sf::Vector2i start_pos(
                game_start_packet.start_x(), game_start_packet.start_y()
        );
        players_list.at(player_id).set_position(start_pos);

        std::vector<std::vector<int>> alibis;
        alibis.reserve(game_start_packet.alibis_size());
        for(auto& alibi : game_start_packet.alibis())
        {
            alibis.emplace_back(alibi.rooms().begin(), alibi.rooms().end());
        }

        int crime_room = game_start_packet.crime_room();
        Item crime_item(game_start_packet.crime_item_name(), game_start_packet.crime_item_description());
        int turns_per_game = game_start_packet.turns_per_game();

        left_panel_title_text->set_string("Loading...");
        game_state_manager.push_state(
                new PlayState(
                        window, game_state_manager, std::move(server_connection), game_board,
                        player_id, players_list, alibis, crime_room, crime_item, turns_per_game
                ), true
        );
    }
    else
    {
        Log::warn() << "Received unhandled packet: " << packet.type_url() << "!" << std::endl;
    }
}

void LobbyState::ready_clicked(Ui::Button& button)
{
    if(player_id == -1)
        return;

    ready = !ready;
    time = 0;
    button.get_child<Ui::Text>()->set_string(ready ? "I'm ready!" : "Not ready");

    Packet::PlayerReadyPacket player_ready_packet;
    player_ready_packet.set_player_id(player_id);
    player_ready_packet.set_ready(ready);
    server_connection.send(player_ready_packet);
    button.get_child<Ui::Text>()->set_color(ready ? Ui::Color::Green : Ui::Color::Red);
}

void LobbyState::exit_clicked(Ui::Button& button)
{
    server_connection.send(Packet::DisconnectPacket());
    game_state_manager.push_state(new MainMenuState(window, game_state_manager), true);
}
