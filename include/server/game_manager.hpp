#pragma once

#include <string>
#include <random>
#include <unordered_map>

#include "remote_player.hpp"
#include "server.hpp"

class GameManager
{
public:
    GameManager();

    void run();

    void packet_received(RemotePlayer& sender, std::unique_ptr<Packet> packet);

private:
    // Server config
    const unsigned short SERVER_PORT = 2704;
    const sf::IpAddress SERVER_ADDR = sf::IpAddress::Any;
    const float COUNTDOWN_INTERVAL = 1;
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

    bool teleport_allowed = false;

    bool enabled = false;
    Server game_server;

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

    int current_player_id = 0;
    std::vector<RemotePlayer> players;

    std::mt19937 gen;
    sf::Clock timer;

    int turn = 0;
    int moves_left;
    int actions_left;

    GameBoard game_board;

    int murderer_id;
    std::vector<int> votes;
};
