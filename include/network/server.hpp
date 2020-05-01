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
    const float COUNTDOWN_INTERVAL = 5;
    const size_t MIN_PLAYERS_COUNT = 1;
    const int MOVES_PER_TURN = 6;
    const int ACTIONS_PER_TURN = 1;
    const int ALIBI_LENGTH = 12;
    const int VISIBLE_ALIBIS = 3;
    const int TURNS_PER_GAME = 2;

    const std::vector<std::string> hours {
        "17:00", "17:30", "18:00", "18:30", "19:00", "19:30", "20:00",
        "20:30", "21:00", "21:30", "22:00", "22:30", "23:00"
    };

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

    int murderer_id;
    std::vector<int> votes;

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
