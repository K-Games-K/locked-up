#include "server/remote_player.hpp"

RemotePlayer::RemotePlayer(int player_id, const std::string& nickname,
    const std::string& avatar_name, Connection&& connection)
    : Player(nickname, avatar_name), player_id(player_id), connection(std::move(connection)) {}

void RemotePlayer::set_player_id(int player_id)
{
    this->player_id = player_id;
}

int RemotePlayer::get_player_id() const
{
    return player_id;
}

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

