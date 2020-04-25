#include <iostream>

#include "utils.hpp"
#include "state/play_state.hpp"
#include "network/packet/packets.hpp"

PlayState::PlayState(sf::RenderWindow& window)
        : GameState(window),
          server_connection(SERVER_ADDR, SERVER_PORT),
          textures("assets/sprites", "png"),
          fonts("assets/fonts", "ttf")
{
    JoinGamePacket packet("General Kenobi");
    player_id = 0;
    server_connection.send(packet);

    player_sprite.setTexture(textures.get("player"));
    bg_sprite.setTexture(textures.get("mapa4"));
}

std::unique_ptr<GameState> PlayState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
    {
        server_connection.send(DisconnectPacket());
        window.close();
    }

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mosepos = sf::Mouse::getPosition(window);

        int posx = (mosepos.x / (float) TILE_SIZE) + camera_pos.x;
        int posy = (mosepos.y / (float) TILE_SIZE) + camera_pos.y;

        server_connection.send(PlayerMovePacket(posx, posy, player_id, false));
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

    // Sizes of viewport in both window and world coordinates.
    const sf::Vector2f window_viewport(window.getSize().x, window.getSize().y);
    const sf::Vector2f world_viewport = window_viewport / (float) TILE_SIZE;

    if(players.size() != 0)
    {
        Player& current_player = players[player_id];
        sf::Vector2f camera_target_pos(
                Utils::clamp(
                        current_player.get_position().x - world_viewport.x / 2 + 0.5, 0,
                        Utils::max(game_board.get_width() - world_viewport.x, 0)),
                Utils::clamp(
                        current_player.get_position().y - world_viewport.y / 2 + 0.5, 0,
                        Utils::max(game_board.get_height() - world_viewport.y, 0))
        );

        camera_pos.x = Utils::lerp(camera_pos.x, camera_target_pos.x, 5 * dt);
        camera_pos.y = Utils::lerp(camera_pos.y, camera_target_pos.y, 5 * dt);

        bg_sprite.setTextureRect(
                {
                        (int) (camera_pos.x * TILE_SIZE), (int) (camera_pos.y * TILE_SIZE),
                        (int) window_viewport.x, (int) window_viewport.y
                }
        );
    }
    else
    {
        bg_sprite.setTextureRect(
                {
                        0, 0,
                        (int) window_viewport.x, (int) window_viewport.y
                }
        );
    }

    if(debug_render)
        bg_sprite.setColor(sf::Color(150, 150, 150));
    else
        bg_sprite.setColor(sf::Color::White);
    window.draw(bg_sprite);

    for(auto& player : players)
    {
        sf::Vector2f current_pos(player_sprite.getPosition());
        sf::Vector2f target_pos(
                (player.get_position().x - camera_pos.x) * TILE_SIZE + 1,
                (player.get_position().y - camera_pos.y) * TILE_SIZE + 1
        );

        player_sprite.setPosition(target_pos);
        window.draw(player_sprite);
    }

    if(debug_render)
    {
        sf::Font font = fonts.get("IndieFlower-Regular");

        for(int y = 0; y < game_board.get_height(); ++y)
        {
            for(int x = 0; x < game_board.get_width(); ++x)
            {
                if(!game_board.can_move(x, y, 1, 0))
                {
                    sf::VertexArray arr(sf::LineStrip, 4);
                    arr[0].position = sf::Vector2f((x + 1 - camera_pos.x) * TILE_SIZE,
                            (y - camera_pos.y) * TILE_SIZE
                    );
                    arr[0].color = sf::Color::Red;
                    arr[1].position = sf::Vector2f((x + 1 - camera_pos.x) * TILE_SIZE,
                            (y + 1 - camera_pos.y) * TILE_SIZE
                    );
                    arr[1].color = sf::Color::Red;
                    arr[2].position = sf::Vector2f((x + 1 - camera_pos.x) * TILE_SIZE + 1,
                            (y + 1 - camera_pos.y) * TILE_SIZE
                    );
                    arr[2].color = sf::Color::Red;
                    arr[3].position = sf::Vector2f((x + 1 - camera_pos.x) * TILE_SIZE + 1,
                            (y - camera_pos.y) * TILE_SIZE + 1
                    );
                    arr[3].color = sf::Color::Red;

                    window.draw(arr);
                }
                if(!game_board.can_move(x, y, 0, 1))
                {
                    sf::VertexArray arr(sf::LineStrip, 4);
                    arr[0].position = sf::Vector2f((x - camera_pos.x) * TILE_SIZE,
                            (y + 1 - camera_pos.y) * TILE_SIZE
                    );
                    arr[0].color = sf::Color::Red;
                    arr[1].position = sf::Vector2f((x + 1 - camera_pos.x) * TILE_SIZE,
                            (y + 1 - camera_pos.y) * TILE_SIZE
                    );
                    arr[1].color = sf::Color::Red;
                    arr[2].position = sf::Vector2f((x + 1 - camera_pos.x) * TILE_SIZE,
                            (y + 1 - camera_pos.y) * TILE_SIZE + 1
                    );
                    arr[2].color = sf::Color::Red;
                    arr[3].position = sf::Vector2f((x - camera_pos.x) * TILE_SIZE,
                            (y + 1 - camera_pos.y) * TILE_SIZE + 1
                    );
                    arr[3].color = sf::Color::Red;

                    window.draw(arr);
                }

                // Rendiring name of room on each tile.
                // std::string str = game_board.get_room(x, y).get_name().substr(0, 6);
                // sf::Text name(str, font, 12);
                // name.setPosition((x - camera_pos.x) * TILE_SIZE + 2, (y - camera_pos.y) * TILE_SIZE + 2);
                // window.draw(name);
            }
        }

        std::string str = "player_id: " + std::to_string(player_id)
                + "\nx: " + std::to_string(players[player_id].get_position().x) + " y: "
                + std::to_string(players[player_id].get_position().y)
                + "\nroom: " + game_board.get_room(
                players[player_id].get_position().x,
                players[player_id].get_position().y
        ).get_name();
        sf::Text text(str, font, 25);
        text.setPosition(5, 5);
        window.draw(text);
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
