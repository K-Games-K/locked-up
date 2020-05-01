#pragma once

#include <string>
#include <random>
#include <unordered_map>
#include <SFML/Network.hpp>

#include "remote_player.hpp"
#include "game_board.hpp"

class Server
{
private:
    const float COUNTDOWN_INTERVAL = 0.5;
    const size_t MIN_PLAYERS_COUNT = 1;
    const int MOVES_PER_TURN = 3;
    const int ACTIONS_PER_TURN = 1;
    const int ALIBI_LENGTH = 12;
    const int VISIBLE_ALIBIS = 3;

    bool enabled = false;
    bool teleport_allowed = false;

    sf::TcpListener listener;

    int current_player_id = 0;
    std::vector<RemotePlayer> players;

    std::mt19937 gen;

    int turn = 0;
    int moves_left;
    int actions_left;

    GameBoard game_board;

    enum class GameStage
    {
        Lobby,
        Countdown,
        NewTurn,
        Movement,
        Action,
        EndTurn,
        Voting,
        Results
    } game_stage = GameStage::Lobby;

    sf::Clock timer;

    void new_connection(Connection connection);

    void packet_received(RemotePlayer& sender, std::unique_ptr<Packet> packet);

    void disconnected(RemotePlayer& player, const std::string& reason);

public:
    Server(unsigned short bind_port, sf::IpAddress bind_addr = sf::IpAddress::Any);

    bool is_enabled() const;

    void update();

    void broadcast(const Packet& packet);
};
