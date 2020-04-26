#include <iostream>

#include "utils.hpp"
#include "state/play_state.hpp"
#include "network/packet/packets.hpp"
#include "ui/button.hpp"

PlayState::PlayState(sf::RenderWindow& window)
    : GameState(window),
    server_connection(SERVER_ADDR, SERVER_PORT),
    textures("assets/sprites", "png"),
    fonts("assets/fonts", "ttf"),
    player_renderer(window, {textures, fonts}),
    game_board_renderer(window, {textures, fonts}),
    debug_renderer(window, {textures, fonts}),
    user_interface_renderer(window, {textures, fonts}),
    user_interface(window, {0, 0}, sf::Vector2f(window.getSize()))
{
    JoinGamePacket packet("General Kenobi");
    player_id = 0;
    server_connection.send(packet);

    user_interface.add_widget(
        new Ui::Button(
            user_interface,
            "This is button!",
            fonts.get("IndieFlower-Regular"),
            [](Ui::Button& btn) {
                std::cout << "Clicked: " << btn.get_text().get_text() << "!" << std::endl;
            },
            {-20, 0},
            {200, 40},
            {sf::Color::White, sf::Color::Green, sf::Color::Red},
            Ui::Anchor::CenterRight,
            Ui::Anchor::CenterRight
        )
    );
}

std::unique_ptr<GameState> PlayState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
    {
        server_connection.send(DisconnectPacket());
        window.close();
    }

    user_interface.handle_event(event);

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(window);
        if(!Utils::is_inside(game_board_position, GAME_BOARD_SIZE, mouse_pos))
            return nullptr;

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
            case sf::Keyboard::Escape:
                window.close();
                break;
            case sf::Keyboard::Tilde:
                debug_render = !debug_render;
                break;
            default:
                break;
        }
    }

    return nullptr;
}

std::unique_ptr<GameState> PlayState::update(float dt)
{
    if(server_connection.is_connected())
    {
        auto packet = server_connection.recv();
        if(packet != nullptr)
            packet_received(std::move(packet));
    }

    return nullptr;
}

void PlayState::render(float dt)
{
    window.clear(CLEAR_COLOR);

    if(players.size() != 0)
    {
        sf::Vector2f game_board_viewport = GAME_BOARD_SIZE / (float) TILE_SIZE;
        Player& current_player = players[player_id];
        sf::Vector2f camera_target_pos(
            Utils::clamp(
                current_player.get_position().x - game_board_viewport.x / 2 + 0.5, 0,
                Utils::max(game_board.get_width() - game_board_viewport.x, 0)),
            Utils::clamp(
                current_player.get_position().y - game_board_viewport.y / 2 + 0.5, 0,
                Utils::max(game_board.get_height() - game_board_viewport.y, 0))
        );

        camera_pos = {
            Utils::lerp(camera_pos.x, camera_target_pos.x, 5 * dt),
            Utils::lerp(camera_pos.y, camera_target_pos.y, 5 * dt)
        };
    }
    else
    {
        camera_pos = {0, 0};
    }

    game_board_renderer.set_game_board_position(game_board_position);
    game_board_renderer.set_game_board_size(GAME_BOARD_SIZE);
    game_board_renderer.render(game_board, camera_pos);

    player_renderer.set_game_board_position(game_board_position);
    player_renderer.render(players, camera_pos);

    user_interface_renderer.render(user_interface);

    if(debug_render)
    {
        debug_renderer.set_game_board_position(game_board_position);
        debug_renderer.set_game_board_size(GAME_BOARD_SIZE);
        debug_renderer.render({player_id, players[player_id], game_board}, camera_pos);
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
        case PlayersListPacket::PACKET_ID:
        {
            auto players_list_packet = dynamic_cast<PlayersListPacket&>(*packet);
            player_id = players_list_packet.get_player_id();
            players = players_list_packet.get_players_list();

            std::cout << "I'm id " << player_id << std::endl;
            std::cout << "Received players list: " << std::endl;
            for(auto& player : players)
                std::cout << "\t - " << player.get_nickname() << std::endl;

            break;
        }
        case PlayerMovePacket::PACKET_ID:
        {
            auto player_move_packet = dynamic_cast<PlayerMovePacket&>(*packet);
            uint16_t player_id = player_move_packet.get_player_id();
            Player& player = players.at(player_id);

            if(player_move_packet.is_relative())
                player.move(player_move_packet.get_x(), player_move_packet.get_y());
            else
                player.set_position(player_move_packet.get_x(), player_move_packet.get_y());

            break;
        }
        case GameBoardPacket::PACKET_ID:
        {
            auto game_board_packet = dynamic_cast<GameBoardPacket&>(*packet);
            game_board = game_board_packet.get_game_board();

            break;
        }
        default:
            break;
    }
}

sf::Vector2f PlayState::window_to_board_coords(sf::Vector2f window_coords)
{
    return (window_coords - game_board_position) / (float) TILE_SIZE + camera_pos;
}

sf::Vector2f PlayState::board_to_window_coords(sf::Vector2f window_coords)
{
    return (window_coords - camera_pos) * (float) TILE_SIZE + game_board_position;
}
