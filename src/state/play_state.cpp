#include <iostream>

#include "state/play_state.hpp"
#include "network/packet/packets.hpp"
#include "network/connection.hpp"

PlayState::PlayState(sf::RenderWindow& window)
        : GameState(window),
          board(GAMEBOARD_WIDTH, GAMEBOARD_HEIGHT),
          server_connection(SERVER_ADDR, SERVER_PORT)
{
    JoinGamePacket packet("General Kenobi");
    std::cout << server_connection.send(packet) << std::endl;
}

std::unique_ptr<GameState> PlayState::handle_input(sf::Event event)
{
    if(event.type == sf::Event::Closed)
        window.close();

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
        default:
            break;
    }
}
