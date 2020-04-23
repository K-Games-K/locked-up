#include <iostream>

#include "state/play_state.hpp"
#include "network/packet/packets.hpp"
#include "network/connection.hpp"
#include "state/play_state.hpp"


PlayState::PlayState(sf::RenderWindow& window)
        : GameState(window),
          board(GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT),
          server_connection(SERVER_ADDR, SERVER_PORT)
{
    JoinGamePacket packet("General Kenobi");
    std::cout << server_connection.send(packet) << std::endl;
    background_txt.loadFromFile("assets/sprites/board.png");

    player_txt.loadFromFile("assets/sprites/player.png");

    players.push_back(Player("V_Ader", 10, 14));
}

std::unique_ptr<GameState> PlayState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
        window.close();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {

        sf::Vector2i mosepos = sf::Mouse::getPosition(this->window);

        int posx = (int)(mosepos.x / 40);
        int posy = (int)(mosepos.y / 40);

        std::cout << "room: " << board.get_room(posx, posy).get_name() << std::endl;
        printf("mose at: %d %d\n", posx, posy);
        players[0].set_position(posx, posy);

    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        players[0].set_position(players[0].get_x(), players[0].get_y() - 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        players[0].set_position(players[0].get_x(), players[0].get_y() + 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        players[0].set_position(players[0].get_x() - 1, players[0].get_y());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        players[0].set_position(players[0].get_x() + 1, players[0].get_y());
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
    sf::Sprite temp;
    temp.setTexture(background_txt);

    window.draw(temp);

    sf::Sprite player_sprite;

    for (Player player_obj : players)
    {
        player_sprite.setTexture(player_txt);

        player_sprite.setPosition(player_obj.get_x() * 40, player_obj.get_y() * 40);

        window.draw(player_sprite);
    }

}

void PlayState::packet_received(std::unique_ptr<Packet> packet)
{
    std::cout << "Server sent a packet!" << std::endl;

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
            player.set_position(player_move_packet.get_x(), player_move_packet.get_y());

            break;
        }
        default:
            break;
    }
}
