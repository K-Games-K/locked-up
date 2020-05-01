#include <iostream>
#include <functional>
#include <sstream>

#include "utils.hpp"
#include "state/play_state.hpp"
#include "network/packet/packets.hpp"

PlayState::PlayState(sf::RenderWindow& window, GameStateManager& game_state_manager,
    Connection server_connection, const GameBoard& game_board, int player_id,
    const std::vector<Player>& players_list, const std::vector<std::vector<int>>& alibis,
    int crime_room, Item crime_item)
    : GameState(window, game_state_manager),
    server_connection(server_connection),
    game_board(std::move(game_board)),
    player_id(player_id),
    players_list(std::move(players_list)),
    background_renderer(window, {textures, fonts}),
    player_renderer(window, {textures, fonts}),
    game_board_renderer(window, {textures, fonts}),
    debug_renderer(window, {textures, fonts}),
    panel_renderer(window, {textures, fonts}),
    user_interface({0, 0}, (sf::Vector2f) window.getSize())
{
    auto& font = fonts.get("IndieFlower-Regular");
    Ui::ButtonColors button_colors = {
        sf::Color(0, 0, 0, 140),
        sf::Color(0, 0, 0, 190),
        sf::Color(0, 0, 0, 210)
    };

    walk_sound.setBuffer(sound_buffers.get("walk_sound"));
    walk_sound.setVolume(40);

    notepad_widget = new Ui::NotepadWidget(
        Notepad(this->players_list, alibis, game_board), textures.get("paper_big"),
        font, {}, {-50, 0},
        Ui::Anchor::CenterRight, Ui::Anchor::CenterRight
    );
    user_interface.add_widget(notepad_widget);

    action_panel = new Ui::Panel(
        {game_board_pos.x, -20}, {900, 50},
        sf::Color::Transparent,
        Ui::Anchor::BottomLeft, Ui::Anchor::BottomLeft
    );
    user_interface.add_widget(action_panel);

    search_action_button = new Ui::Button(
        "Search current room",
        font,
        std::bind(&PlayState::action_clicked, this, std::placeholders::_1),
        {0, 0}, {300, 40},
        button_colors,
        {},
        Ui::Anchor::Center, Ui::Anchor::Center
    );
    action_panel->add_widget(search_action_button);

    notification_widget = new Ui::NotificationWidget(
        font,
        {0, 40}, {400, 100},
        {sf::Color::White, 30, sf::Color::Black, 1},
        Ui::Anchor::CenterTop, Ui::Anchor::CenterTop
    );
    notification_widget->show_notification("Hello there general!", 2);
    user_interface.add_widget(notification_widget);

    popup = new Ui::Popup(
        textures.get("paper_small"),
        font,
        {-100, 0},
        Ui::Anchor::Center, Ui::Anchor::Center
    );
    user_interface.add_widget(popup);

    voting_menu = new Ui::Panel(
        {0, 0}, user_interface.get_size(),
        sf::Color(0, 0, 0, 220)
    );
    voting_menu->set_enabled(false);
    user_interface.add_widget(voting_menu);

    auto voting_menu_panel = new Ui::TexturedPanel(
        textures.get("paper"),
        {0, 0},
        Ui::Anchor::Center, Ui::Anchor::Center
    );
    voting_menu->add_widget(voting_menu_panel);

    voting_menu_panel->add_widget(
        new Ui::Text(
            "Police has arrived!\nNow you'll need to vote\nwho are you going to accuse:",
            font,
            {0, 40},
            {sf::Color::Black, 28},
            Ui::Anchor::CenterTop, Ui::Anchor::CenterTop
        )
    );

    for(int i = 0; i < this->players_list.size(); ++i)
    {
        auto& player = players_list[i];
        voting_menu_panel->add_widget(
            new Ui::Button(
                player.get_nickname(),
                font,
                std::bind(&PlayState::vote_clicked, this, std::placeholders::_1),
                {0, (i * 50.0f) - ((this->players_list.size() - 1) * 25.0f)}, {420, 40},
                button_colors,
                {sf::Color::Black},
                Ui::Anchor::Center, Ui::Anchor::Center
            )
        );
    }

    pause_menu = new Ui::Panel(
        {0, 0}, user_interface.get_size(),
        sf::Color(0, 0, 0, 180)
    );
    pause_menu->set_enabled(false);
    user_interface.add_widget(pause_menu);

    auto pause_menu_panel = new Ui::TexturedPanel(
        textures.get("paper"),
        {0, 0},
        Ui::Anchor::Center, Ui::Anchor::Center
    );
    pause_menu->add_widget(pause_menu_panel);

    pause_menu_panel->add_widget(
        new Ui::Text(
            "Game Paused",
            font,
            {0, 30},
            {sf::Color::Black, 50},
            Ui::Anchor::CenterTop, Ui::Anchor::CenterTop
        )
    );

    pause_menu_panel->add_widget(
        new Ui::Button(
            "Resume",
            font,
            std::bind(&PlayState::resume_clicked, this, std::placeholders::_1),
            {0, -10}, {420, 40},
            button_colors,
            {sf::Color::Black},
            Ui::Anchor::CenterBottom, Ui::Anchor::Center
        )
    );
    pause_menu_panel->add_widget(
        new Ui::Button(
            "Exit",
            font,
            std::bind(&PlayState::exit_clicked, this, std::placeholders::_1),
            {0, 10}, {420, 40},
            button_colors,
            {sf::Color::Black},
            Ui::Anchor::CenterTop, Ui::Anchor::Center
        )
    );

    popup->set_title("Dead body found!");
    std::stringstream descr;
    descr << "A dead body was found in " << game_board.get_room(crime_room).get_name() << ".\n";
    descr << crime_item.get_name() << " was found by the side.";
    popup->set_description(descr.str());
    popup->show();
}

void PlayState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
    {
        server_connection.send(DisconnectPacket());
        game_state_manager.pop_state();
    }

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left &&
        !paused)
    {
        sf::Vector2f mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(window);
        sf::FloatRect game_board_rect(game_board_pos, GAME_BOARD_SIZE);

        if(game_board_rect.contains(mouse_pos))
        {
            sf::Vector2i world_mouse_pos = (sf::Vector2i) window_to_board_coords(mouse_pos);

            server_connection.send(
                PlayerMovePacket(
                    world_mouse_pos.x,
                    world_mouse_pos.y,
                    player_id,
                    false
                )
            );
        }
    }

    sf::Vector2f mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(window);
    sf::Vector2f mouse_pos_rel = mouse_pos - user_interface.get_relative_position(
        {0, 0},
        (sf::Vector2f) window.getSize()
    );
    user_interface.handle_event(event, mouse_pos_rel);

    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Up:
                server_connection.send(PlayerMovePacket(0, -1, player_id));
                break;
            case sf::Keyboard::Down:
                server_connection.send(PlayerMovePacket(0, 1, player_id));
                break;
            case sf::Keyboard::Left:
                server_connection.send(PlayerMovePacket(-1, 0, player_id));
                break;
            case sf::Keyboard::Right:
                server_connection.send(PlayerMovePacket(1, 0, player_id));
                break;
            case sf::Keyboard::Tilde:
                debug_render = !debug_render;
                break;
            case sf::Keyboard::Escape:
                paused = !paused;
                pause_menu->set_enabled(paused);
                break;
            default:
                break;
        }
    }
}

void PlayState::update(float dt)
{
    if(server_connection.is_connected())
    {
        auto packet = server_connection.recv();
        if(packet != nullptr)
            packet_received(std::move(packet));
    }

    user_interface.update(dt);
}

void PlayState::render(float dt)
{
    window.clear(CLEAR_COLOR);

    sf::Vector2f game_board_viewport = GAME_BOARD_SIZE / (float) TILE_SIZE;
    Player& current_player = players_list.at(current_player_id);
    sf::Vector2f camera_target_pos(
        Utils::clamp(
            current_player.get_position().x - game_board_viewport.x / 2 + 0.5, 0,
            Utils::max(game_board.get_width() - game_board_viewport.x, 0)),
        Utils::clamp(
            current_player.get_position().y - game_board_viewport.y / 2 + 0.5, 0,
            Utils::max(game_board.get_height() - game_board_viewport.y, 0))
    );

    camera_pos = {
        Utils::lerp(camera_pos.x, camera_target_pos.x, dt / CAMERA_SMOOTH),
        Utils::lerp(camera_pos.y, camera_target_pos.y, dt / CAMERA_SMOOTH)
    };

    background_renderer.render(textures.get("background"), dt);

    game_board_renderer.set_camera_pos(camera_pos);
    game_board_renderer.set_game_board_pos(game_board_pos);
    game_board_renderer.set_game_board_size(GAME_BOARD_SIZE);
    game_board_renderer.render(game_board, dt);

    player_renderer.set_camera_pos(camera_pos);
    player_renderer.set_game_board_pos(game_board_pos);
    player_renderer.set_game_board_size(GAME_BOARD_SIZE);
    player_renderer.render(players_list, dt);

    panel_renderer.render(user_interface, dt);

    if(debug_render)
    {
        debug_renderer.set_camera_pos(camera_pos);
        debug_renderer.set_game_board_pos(game_board_pos);
        debug_renderer.set_game_board_size(GAME_BOARD_SIZE);
        debug_renderer.render({player_id, players_list.at(player_id), game_board}, dt);
    }
}

void PlayState::packet_received(std::unique_ptr<Packet> packet)
{
    switch(packet->get_id())
    {
        case DebugPacket::PACKET_ID:
        {
            auto debug_packet = dynamic_cast<DebugPacket&>(*packet);
            std::cout << "[Debug:" << server_connection.get_addr() << "]: "
                << debug_packet.get_message() << std::endl;

            break;
        }
        case PlayerMovePacket::PACKET_ID:
        {
            auto player_move_packet = dynamic_cast<PlayerMovePacket&>(*packet);
            uint16_t player_id = player_move_packet.get_player_id();
            Player& player = players_list.at(player_id);

            walk_sound.play();
            if(player_move_packet.is_relative())
                player.move(player_move_packet.get_x(), player_move_packet.get_y());
            else
                player.set_position(player_move_packet.get_x(), player_move_packet.get_y());

            break;
        }
        case NewTurnPacket::PACKET_ID:
        {
            auto new_turn_packet = dynamic_cast<NewTurnPacket&>(*packet);
            current_player_id = new_turn_packet.get_current_player_id();

            std::stringstream ss;
            ss << "New turn!\n";
            ss << "Current player: " << players_list.at(current_player_id).get_nickname();
            notification_widget->show_notification(ss.str(), 5);

            break;
        }
        case ClueFoundPacket::PACKET_ID:
        {
            auto clue_found_packet = dynamic_cast<ClueFoundPacket&>(*packet);
            std::string clue = clue_found_packet.get_clue();
            std::string time = clue_found_packet.get_time();

            if(!clue.empty())
            {
                popup->set_title("Found an item!");
                std::stringstream descr;
                descr << "You found a clue that ";
                descr << clue << " was here at " << time << "!";
                popup->set_description(descr.str());
                popup->show();
            }
            else
            {
                popup->set_title("Oops!");
                popup->set_description("You didn't find anything here.");
                popup->show();
            }

            break;
        }
        default:
            break;
    }
}

void PlayState::resume_clicked(Ui::Button& button)
{
    paused = false;
    pause_menu->set_enabled(false);
}

void PlayState::exit_clicked(Ui::Button& button)
{
    server_connection.send(DisconnectPacket());
    game_state_manager.pop_state();
}

void PlayState::action_clicked(Ui::Button& button)
{
    if(button == *search_action_button)
    {
        server_connection.send(ActionPacket(ActionType::SearchRoom));
    }
}

void PlayState::vote_clicked(Ui::Button& button)
{

}

sf::Vector2f PlayState::window_to_board_coords(sf::Vector2f window_coords)
{
    return (window_coords - game_board_pos) / (float) TILE_SIZE + camera_pos;
}

sf::Vector2f PlayState::board_to_window_coords(sf::Vector2f window_coords)
{
    return (window_coords - camera_pos) * (float) TILE_SIZE + game_board_pos;
}
