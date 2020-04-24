#include "remote_player.hpp"

RemotePlayer::RemotePlayer(Connection connection)
        : Player("[invalid]"), connection(connection)
{}

Connection& RemotePlayer::get_connection()
{
    return connection;
}

bool RemotePlayer::operator==(const RemotePlayer& other) const
{
    return connection == other.connection;
}

