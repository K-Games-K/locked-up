#include <functional>
#include <sstream>

#include "logging.hpp"
#include "utils.hpp"
#include "state/play_state.hpp"
#include "state/lobby_state.hpp"

PlayState::PlayState(sf::RenderWindow& window, GameStateManager& game_state_manager,
    Connection&& server_connection, const GameBoard& game_board, int player_id,
    const std::vector<Player>& players_list, const std::vector<std::vector<int>>& alibis,
    int crime_room, Item crime_item, int turns_per_game)
    : GameState(window, game_state_manager),
    server_connection(std::move(server_connection)),
    game_board(std::move(game_board)),
    player_id(player_id),
    players_list(std::move(players_list)),
    background_renderer(window, {textures, fonts}),
    player_renderer(window, {textures, fonts}),
    game_board_renderer(window, {textures, fonts}),
    debug_renderer(window, {textures, fonts}),
    master_widget_renderer(window),
    turns_per_game(turns_per_game)
{
    auto& font = fonts.get("IndieFlower-Regular");
    auto default_color = Ui::Color(0, 0, 0, 140);
    auto hover_color = Ui::Color(0, 0, 0, 190);
    auto active_color = Ui::Color(0, 0, 0, 210);

    walk_sound.setBuffer(sound_buffers.get("walk_sound"));
    walk_sound.setVolume(40);

    click_sound.setBuffer(sound_buffers.get("click_sound"));
    click_sound.setVolume(20);

    user_interface.set_size((sf::Vector2f) window.getSize());

    notepad_widget = (Ui::NotepadWidget*) user_interface.add_widget(
        Ui::NotepadWidget(
            Notepad(this->players_list, alibis, game_board),
            textures.get("paper_big"),
            font
        )
            .set_position({-50, 0})
            .set_origin(Ui::Origin::CenterRight)
            .set_anchor(Ui::Anchor::CenterRight)
    );

    action_panel = (Ui::Panel*) user_interface.add_widget(
        Ui::Panel()
            .set_position({game_board_pos.x, -20})
            .set_size({900, 50})
            .set_origin(Ui::Origin::BottomLeft)
            .set_anchor(Ui::Anchor::BottomLeft)
    );

    search_action_button = (Ui::Button*) action_panel->add_widget(
        Ui::Button()
            .set_callback(std::bind(&PlayState::search_action_clicked, this, std::placeholders::_1))
            .set_default_color(default_color)
            .set_hover_color(hover_color)
            .set_active_color(active_color)
            .set_position({-160, 0})
            .set_size({300, 40})
    );
    search_action_button->add_widget(
        Ui::Text(font, "Search current room")
            .set_color(Ui::Color::White)
    );

    place_clue_action_button = (Ui::Button*) action_panel->add_widget(
        Ui::Button()
            .set_callback(std::bind(&PlayState::place_clue_clicked, this, std::placeholders::_1))
            .set_default_color(default_color)
            .set_hover_color(hover_color)
            .set_active_color(active_color)
            .set_position({160, 0})
            .set_size({300, 40})
            .set_enabled(false)
    );
    place_clue_action_button->add_widget(
        Ui::Text(font, "Place fake clue")
            .set_color(Ui::Color::White)
    );
    

    current_room_text = (Ui::Text*) user_interface.add_widget(
        Ui::Text(font)
            .set_color(Ui::Color::White)
            .set_font_size(30)
            .set_position({-300, 20})
            .set_origin(Ui::Origin::CenterTop)
            .set_anchor(Ui::Anchor::CenterTop)
    );

    notification_widget = (Ui::NotificationWidget*) user_interface.add_widget(
        Ui::NotificationWidget(font)
            .set_position({0, 40})
            .set_size({400, 100})
            .set_origin(Ui::Origin::CenterTop)
            .set_anchor(Ui::Anchor::CenterTop)
    );
    notification_widget->get_text()
        .set_color(Ui::Color::White)
        .set_font_size(30)
        .set_outline_thickness(1);

    voting_menu = (Ui::Panel*) user_interface.add_widget(
        Ui::Panel()
            .set_background_color(Ui::Color(0, 0, 0, 200))
            .set_size((sf::Vector2f) window.getSize())
            .set_enabled(false)
    );
    auto voting_menu_panel = voting_menu->add_widget(Ui::Panel(textures.get("paper")));
    voting_menu_panel->add_widget(
        Ui::Text(font, "Police has arrived!\nNow you'll need to vote\nwho are you going to accuse:")
            .set_font_size(28)
            .set_position({0, 40})
            .set_origin(Ui::Origin::CenterTop)
            .set_anchor(Ui::Anchor::CenterTop)
    );

    for(int i = 0; i < this->players_list.size(); ++i)
    {
        auto& player = players_list[i];
        auto button = voting_menu_panel->add_widget(
            Ui::Button()
                .set_callback(std::bind(&PlayState::vote_clicked, this, std::placeholders::_1))
                .set_default_color(default_color)
                .set_hover_color(hover_color)
                .set_active_color(active_color)
                .set_position({0, (i * 50.0f) - ((this->players_list.size() - 1) * 25.0f)})
                .set_size({420, 40})
        );
        button->add_widget(Ui::Text(font, player.get_nickname()));
    }

    popup = (Ui::Popup*) user_interface.add_widget(
        Ui::Popup(textures.get("paper_small"), font)
            .set_position({-100, 0})
    );

    pause_menu = (Ui::Panel*) user_interface.add_widget(
        Ui::Panel()
            .set_background_color(Ui::Color(0, 0, 0, 180))
            .set_size((sf::Vector2f) window.getSize())
            .set_enabled(false)
    );

    auto pause_menu_panel = pause_menu->add_widget(Ui::Panel(textures.get("paper")));
    pause_menu_panel->add_widget(
        Ui::Text(font, "Game Paused")
            .set_font_size(50)
            .set_position({0, 30})
            .set_origin(Ui::Origin::CenterTop)
            .set_anchor(Ui::Anchor::CenterTop)
    );
    auto resume_button = pause_menu_panel->add_widget(
        Ui::Button()
            .set_callback(std::bind(&PlayState::resume_clicked, this, std::placeholders::_1))
            .set_default_color(default_color)
            .set_hover_color(hover_color)
            .set_active_color(active_color)
            .set_position({0, -10})
            .set_size({420, 40})
            .set_origin(Ui::Origin::CenterBottom)
    );
    resume_button->add_widget(Ui::Text(font, "Resume"));
    auto exit_button = pause_menu_panel->add_widget(
        Ui::Button()
            .set_callback(std::bind(&PlayState::exit_clicked, this, std::placeholders::_1))
            .set_default_color(default_color)
            .set_hover_color(hover_color)
            .set_active_color(active_color)
            .set_position({0, 10})
            .set_size({420, 40})
            .set_origin(Ui::Origin::CenterTop)
    );
    exit_button->add_widget(Ui::Text(font, "Exit"));

    std::stringstream descr;
    descr << "A dead body was found in " << game_board.get_room(crime_room).get_name() << ".\n";
    descr << crime_item.get_name() << " was found by the side.";
    popup->show("Dead body found!", descr.str());

    minimap_panel = (Ui::Panel*) user_interface.add_widget(
        Ui::Panel()
            .set_background_color(Ui::Color(0, 0, 0, 180))
            .set_size((sf::Vector2f) window.getSize())
            .set_enabled(false)
    );
    auto minimap_panel_map = minimap_panel->add_widget(
        Ui::Panel(textures.get("minimap"))
        .set_position({0,0})
    );

    fake_clue_menu = (Ui::Panel*) user_interface.add_widget(
        Ui::Panel()
            .set_background_color(Ui::Color(0, 0, 0, 180))
            .set_size((sf::Vector2f) window.getSize())
            .set_enabled(false)
    );

    auto fake_clue_panel = fake_clue_menu->add_widget(Ui::Panel(textures.get("paper")));
    fake_clue_panel->add_widget(
        Ui::Text(font, "This is the fake clue!")
        .set_font_size(28)
        .set_position({ 0, 40 })
        .set_origin(Ui::Origin::CenterTop)
        .set_anchor(Ui::Anchor::CenterTop)
    );

    for (int i = 0; i < this->hours.size(); ++i)
    {
        auto& hour = hours[i];
        auto button = fake_clue_panel->add_widget(
            Ui::Button()
            .set_callback(
                [this, i](Ui::Button& button) { hour_choice(i); }
            )
            .set_default_color(default_color)
            .set_hover_color(hover_color)
            .set_active_color(active_color)
            .set_position({ -150, (i * 35.0f) - ((this->players_list.size() - 1) * 20.0f) -200})
            .set_size({ 150, 30 })
        );
        button->add_widget(Ui::Text(font, hour));
    }

    for (int i = 0; i < this->players_list.size(); ++i)
    {
        auto& player = players_list[i];
        auto button = fake_clue_panel->add_widget(
            Ui::Button()
            .set_callback(
                [this, i](Ui::Button& button) { fake_player_choice(i); }
            )
            .set_default_color(default_color)
            .set_hover_color(hover_color)
            .set_active_color(active_color)
            .set_position({ 100, (i * 35.0f) - ((this->players_list.size() - 1) * 20.0f) -200})
            .set_size({ 150, 30 })
        );
        button->add_widget(Ui::Text(font, player.get_nickname()));
    }
    auto button = fake_clue_menu->add_widget(
        Ui::Button()
        .set_callback(std::bind(&PlayState::place_clue_send_clicked, this, std::placeholders::_1))
        .set_default_color(default_color)
        .set_hover_color(hover_color)
        .set_active_color(active_color)
        .set_position({ 0, 300 })
        .set_size({ 420, 40 })
        .set_origin(Ui::Origin::CenterBottom)
    );
    button->add_widget(
        Ui::Text(font, "Place fake clue!")
        .set_color(Ui::Color::White)
    );

}

void PlayState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
    {
        server_connection.send(Packet::DisconnectPacket());
        game_state_manager.pop_state();
    }

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        click_sound.play();

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left &&
        !paused)
    {
        sf::Vector2f mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(window);
        sf::FloatRect game_board_rect(game_board_pos, GAME_BOARD_SIZE);

        if(game_board_rect.contains(mouse_pos))
        {
            sf::Vector2i world_mouse_pos = (sf::Vector2i) window_to_board_coords(mouse_pos);

            Packet::PlayerMovePacket player_move_packet;
            player_move_packet.set_x(world_mouse_pos.x);
            player_move_packet.set_y(world_mouse_pos.y);
            player_move_packet.set_player_id(player_id);
            server_connection.send(player_move_packet);
        }
    }

    user_interface.handle_event(
        event, (sf::Vector2f) sf::Mouse::getPosition(window),
        {0, 0}, (sf::Vector2f) window.getSize()
    );

    if(event.type == sf::Event::KeyPressed)
    {
        // TODO: This shouldn't be a lambda function, probably.
        auto construct_move_packet = [](int x, int y, int player_id) {
            Packet::PlayerMovePacket player_move_packet;
            player_move_packet.set_x(x);
            player_move_packet.set_y(y);
            player_move_packet.set_player_id(player_id);
            player_move_packet.set_relative(true);
            return player_move_packet;
        };

        switch(event.key.code)
        {
            case sf::Keyboard::Up:
                server_connection.send(construct_move_packet(0, -1, player_id));
                break;
            case sf::Keyboard::Down:
                server_connection.send(construct_move_packet(0, 1, player_id));
                break;
            case sf::Keyboard::Left:
                server_connection.send(construct_move_packet(-1, 0, player_id));
                break;
            case sf::Keyboard::Right:
                server_connection.send(construct_move_packet(1, 0, player_id));
                break;
            case sf::Keyboard::Tilde:
                debug_render = !debug_render;
                break;
            case sf::Keyboard::F3:
                paused = !paused;
                minimap_panel->set_enabled(paused);
                break;
            case sf::Keyboard::Escape:
                paused = !paused;
                fake_clue_menu->set_enabled(false);
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
        if(packet.has_value())
            packet_received(std::move(*packet));
    }

    auto& current_room = game_board.get_room(
        players_list.at(player_id).get_position().x,
        players_list.at(player_id).get_position().y
    );

    current_room_text->set_string(
        "Current player: " + players_list[current_player_id].get_nickname() + "    Current room: " +
        current_room.get_name() + "   Turn: " + std::to_string(current_turn) + "/" + std::to_string(turns_per_game)
    );

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

    if(debug_render)
    {
        debug_renderer.set_camera_pos(camera_pos);
        debug_renderer.set_game_board_pos(game_board_pos);
        debug_renderer.set_game_board_size(GAME_BOARD_SIZE);
        debug_renderer.render({player_id, players_list.at(player_id), game_board}, dt);
    }

    master_widget_renderer.render(user_interface, dt, {0, 0}, (sf::Vector2f) window.getSize());
}

void PlayState::packet_received(const Packet::Any& packet)
{
    if(packet.Is<Packet::DebugPacket>())
    {
        Packet::DebugPacket debug_packet;
        packet.UnpackTo(&debug_packet);

        Log::debug() << "Server: " << debug_packet.debug_msg() << std::endl;
    }
    else if(packet.Is<Packet::PlayerMovePacket>())
    {
        Packet::PlayerMovePacket player_move_packet;
        packet.UnpackTo(&player_move_packet);

        Player& player = players_list.at(player_move_packet.player_id());

        walk_sound.play();
        if(player_move_packet.relative())
            player.move(player_move_packet.x(), player_move_packet.y());
        else
            player.set_position(player_move_packet.x(), player_move_packet.y());
    }
    else if(packet.Is<Packet::NewTurnPacket>())
    {
        Packet::NewTurnPacket new_turn_packet;
        packet.UnpackTo(&new_turn_packet);

        current_player_id = new_turn_packet.current_player_id();
        current_turn = new_turn_packet.current_turn();

        // std::stringstream ss;
        // ss << "New turn!\n";
        // ss << "Current player: " << players_list.at(current_player_id).get_nickname();
        // notification_widget->show_notification(ss.str(), 2.5);
    }
    else if(packet.Is<Packet::ClueFoundPacket>())
    {
        Packet::ClueFoundPacket clue_found_packet;
        packet.UnpackTo(&clue_found_packet);

        std::string clue = clue_found_packet.clue();
        std::string time = clue_found_packet.time();

        if(!clue.empty())
        {
            std::stringstream descr;
            descr << "You found a clue that\n";
            descr << clue << " was here at " << time << "!";
            popup->show("Found an item!", descr.str());
        }
        else
        {
            popup->show("Oops!", "You didn't find anything here.");
        }
    }
    else if(packet.Is<Packet::MurdererPacket>())
    {
        Packet::MurdererPacket murderer_packet;
        packet.UnpackTo(&murderer_packet);

        notification_widget->show("You are the MURDERER!", 5);
        place_clue_action_button->set_enabled(true);
    }
    else if(packet.Is<Packet::VotePacket>())
    {
        Packet::VotePacket vote_packet;
        packet.UnpackTo(&vote_packet);

        voting_menu->set_enabled(true);
    }
    else if(packet.Is<Packet::GameResultsPacket>())
    {
        Packet::GameResultsPacket game_results_packet;
        packet.UnpackTo(&game_results_packet);

        std::string murderer = game_results_packet.murderer();
        std::string voting_result = game_results_packet.voting_result();
        std::vector<int> voting_points(
            game_results_packet.voting_result_points().begin(),
            game_results_packet.voting_result_points().end());
        std::string winner = game_results_packet.winner();


        std::stringstream descr;
        descr << murderer << " was the murderer and you accused: " << std::endl;
        for (int i = 0; i < voting_points.size(); ++i)
        {
            descr << players_list[i].get_nickname() << " : " << voting_points[i] << std::endl;
        }
        descr << "The game wins " << winner << ".";
 
        popup->set_close_callback(
                [this](Ui::Popup&) {
                    game_state_manager.push_state(
                            new LobbyState(
                                    window, game_state_manager,
                                    std::move(server_connection),
                                    players_list[player_id].get_nickname(),
                                    players_list[player_id].get_avatar_name()
                            ), true
                    );
                }
        );
        popup->show("Game results:", descr.str());
    }
    else
    {
        Log::warn() << "Received unhandled packet: " << packet.type_url() << "!" << std::endl;
    }
}

void PlayState::resume_clicked(Ui::Button& button)
{
    paused = false;
    pause_menu->set_enabled(false);
}

void PlayState::exit_clicked(Ui::Button& button)
{
    server_connection.send(Packet::DisconnectPacket());
    game_state_manager.pop_state();
}

void PlayState::search_action_clicked(Ui::Button& button)
{
    server_connection.send(Packet::ClueFoundPacket());
    
}

void PlayState::place_clue_clicked(Ui::Button& button)
{
    paused = true;
    fake_clue_menu->set_enabled(paused);
}

void PlayState::place_clue_send_clicked(Ui::Button& button)
{
    Packet::FakeCluePacket fake_clue_packet;
    fake_clue_packet.set_time(fake_hour);
    fake_clue_packet.set_player_id(fake_player_id);
    server_connection.send(fake_clue_packet);

    paused = false;
    fake_clue_menu->set_enabled(paused);
}

void PlayState::vote_clicked(Ui::Button& button)
{
    if(voted)
        return;

    std::string nickname = button.get_child<Ui::Text>()->get_string();
    int vote_id = std::distance(
        players_list.begin(),
        std::find(players_list.begin(), players_list.end(), Player(nickname))
    );

    Packet::VotePacket vote_packet;
    vote_packet.set_player_id(vote_id);
    server_connection.send(vote_packet);
    voted = true;
    voting_menu->set_enabled(false);
}

sf::Vector2f PlayState::window_to_board_coords(sf::Vector2f window_coords)
{
    return (window_coords - game_board_pos) / (float) TILE_SIZE + camera_pos;
}

sf::Vector2f PlayState::board_to_window_coords(sf::Vector2f window_coords)
{
    return (window_coords - camera_pos) * (float) TILE_SIZE + game_board_pos;
}

void PlayState::hour_choice(int hour)
{
    this->fake_hour = hour;
}

void PlayState::fake_player_choice(int player_id)
{
    this->fake_player_id = player_id;
}
