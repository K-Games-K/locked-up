#pragma once

#include <string>
#include <random>
#include <unordered_map>

#include "console_interface.hpp"
#include "remote_player.hpp"
#include "server.hpp"

class GameManager
{
public:
    GameManager();

    void run();

private:
    // Server config
    const unsigned short SERVER_PORT = 2704;
    const sf::IpAddress SERVER_ADDR = sf::IpAddress::Any;
    float COUNTDOWN_INTERVAL = 1;
    size_t MIN_PLAYERS_COUNT = 1;
    int MOVES_PER_TURN = 6;
    int ACTIONS_PER_TURN = 1;
    int ALIBI_LENGTH = 13;
    int VISIBLE_ALIBIS = 3;
    int TURNS_PER_GAME = 10;
    const std::vector<std::string> hours {
        "17:00", "17:30", "18:00", "18:30", "19:00", "19:30", "20:00",
        "20:30", "21:00", "21:30", "22:00", "22:30", "23:00"
    };

    bool teleport_allowed = false;

    volatile bool enabled = false;
    Server game_server;
    ConsoleInterfrace console_interfrace;

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

    std::mt19937 gen;

    GameBoard game_board;

    sf::Clock timer;
    int current_player_id = 0;
    int turn = 0;
    int moves_left;
    int actions_left;
    int murderer_id;
    std::vector<int> votes;
    std::set<int> pmove_pos;

    void prepare_new_game();

    void packet_received(RemotePlayer& sender, const Packet::Any& packet);

    void set(const std::vector<std::string>& args);

    void stop();

    std::set<int> gen_pmove_pos(int x, int y, int move_count, std::set<int>& pmove_pos);
};
