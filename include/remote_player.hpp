#pragma once

#include "player.hpp"
#include "network/connection.hpp"

class RemotePlayer : public Player
{
private:
    Connection connection;

public:
    explicit RemotePlayer(Connection connection);

    Connection& get_connection();

    bool operator==(const RemotePlayer& other);
};
