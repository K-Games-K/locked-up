#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Network.hpp>

#include "remote_player.hpp"
#include "game_board.hpp"

class Server
{
private:
    const float COUNTDOWN_INTERVAL = 5;
    const size_t MIN_PLAYERS_COUNT = 1;

    bool enabled = false;

    sf::TcpListener listener;

    std::vector<RemotePlayer> players;

    GameBoard game_board;

    enum class GameStage
    {
        Lobby,
        Countdown,
        Play,
        Voting,
        Results
    } game_stage = GameStage::Lobby;

    sf::Clock timer;

    void new_connection(Connection connection);

    void packet_received(RemotePlayer& sender, std::unique_ptr<Packet> packet);

    void disconnected(RemotePlayer& player, const std::string& reason);

public:
    Server(unsigned short bind_port, sf::IpAddress bind_addr = sf::IpAddress::Any);

    void update();

    void broadcast(const Packet& packet);
};
