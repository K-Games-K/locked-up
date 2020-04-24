#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Network.hpp>

#include "remote_player.hpp"
#include "game_board.hpp"

class Server
{
private:
    const int GAMEBOARD_WIDTH = 20;
    const int GAMEBOARD_HEIGHT = 20;

    sf::TcpListener listener;

    std::vector<RemotePlayer> players;

    GameBoard game_board;

    void new_connection(Connection connection);

    void packet_received(RemotePlayer& sender, std::unique_ptr<Packet> packet);

    void lost_connection(RemotePlayer& player);

    void disconnected(RemotePlayer& player, const std::string& reason);

public:
    Server(unsigned short bind_port, sf::IpAddress bind_addr = sf::IpAddress::Any);

    void update();

    void broadcast(const Packet& packet);
};
