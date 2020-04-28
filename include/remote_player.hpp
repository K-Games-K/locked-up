#pragma once

#include "player.hpp"
#include "network/connection.hpp"

class RemotePlayer : public Player
{
private:
    int player_id;
    Connection connection;

    bool ready = false;

public:
    explicit RemotePlayer(int player_id, Connection connection);

    void set_player_id(int player_id);

    int get_player_id() const;

    Connection& get_connection();

    void set_ready(bool ready);

    bool is_ready() const;

    bool operator==(const RemotePlayer& other) const;
};
