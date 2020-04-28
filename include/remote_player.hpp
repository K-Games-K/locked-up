#pragma once

#include "player.hpp"
#include "network/connection.hpp"

class RemotePlayer : public Player
{
private:
    Connection connection;

    bool ready = false;

public:
    explicit RemotePlayer(Connection connection);

    Connection& get_connection();

    void set_ready(bool ready);

    bool is_ready() const;

    bool operator==(const RemotePlayer& other) const;
};
