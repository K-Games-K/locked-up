#include "remote_player.hpp"

RemotePlayer::RemotePlayer(Connection connection)
        : Player("[invalid]"), connection(connection)
{}

Connection& RemotePlayer::get_connection()
{
    return connection;
}

void RemotePlayer::set_ready(bool ready)
{
    this->ready = ready;
}

bool RemotePlayer::is_ready() const
{
    return ready;
}

bool RemotePlayer::operator==(const RemotePlayer& other) const
{
    return connection == other.connection;
}

