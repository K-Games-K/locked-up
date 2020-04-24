#include <iostream>

#include "state/play_state.hpp"
#include "network/packet/packets.hpp"
#include "network/connection.hpp"
#include "state/play_state.hpp"

PlayState::PlayState(sf::RenderWindow& window)
        : GameState(window),
          server_connection(SERVER_ADDR, SERVER_PORT),
          textures("assets/sprites", "png")
{
    JoinGamePacket packet("General Kenobi");
    current_player_id = 0;
    server_connection.send(packet);
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

        int posx = (int) (mosepos.x / 40);
        int posy = (int) (mosepos.y / 40);

        std::cout << "room: " << game_board.get_room(posx, posy).get_name() << std::endl;

        server_connection.send(PlayerMovePacket(posx, posy, false));
    }

    if(event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Up:
                server_connection.send(PlayerMovePacket(0, -1));
                break;
            case sf::Keyboard::Down:
                server_connection.send(PlayerMovePacket(0, 1));
                break;
            case sf::Keyboard::Left:
                server_connection.send(PlayerMovePacket(-1, 0));
                break;
            case sf::Keyboard::Right:
                server_connection.send(PlayerMovePacket(1, 0));
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
    sf::Sprite bg_sprite;
    bg_sprite.setTexture(textures.get("mapa4"));

    int map_render_pos_x = 0;
    int map_render_pos_y = 0;
    bool set_x;
    bool set_y;

    const int map_render_size_x = 17;
    const int map_render_size_y = 17;

    if (players.size() != 0)
    {
        map_render_pos_x = (players[current_player_id].get_x() - 9);
        map_render_pos_y = (players[current_player_id].get_y() - 9);
      

        if (map_render_pos_x < 0) map_render_pos_x = 0;
        else if (map_render_pos_x + map_render_size_x >= game_board.get_width()) map_render_pos_x = game_board.get_width() - map_render_size_x;


        if (map_render_pos_y < 0) map_render_pos_y = 0;
        else if (map_render_pos_y + map_render_size_y >= game_board.get_height()) map_render_pos_y = game_board.get_height() - map_render_size_y;



        bg_sprite.setPosition(-map_render_pos_x * 40, -map_render_pos_y * 40);
    }
    else
    {
        bg_sprite.setPosition(0,0);
    }

    window.draw(bg_sprite);

    sf::Sprite player_sprite;
    player_sprite.setTexture(textures.get("player"));

    

    for(int i = 0; i < players.size(); ++i)
    {
        if (i == current_player_id)
        {
            player_sprite.setPosition((players[current_player_id].get_x() - map_render_pos_x) * 40,( players[current_player_id].get_y() - map_render_pos_y) * 40); //current player
            window.draw(player_sprite);
            continue;
        }

        player_sprite.setPosition(players[i].get_x() * 40, players[i].get_y() * 40);
        window.draw(player_sprite);
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
            players = players_list_packet.get_players_list();

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
