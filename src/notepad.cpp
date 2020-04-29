#include "notepad.hpp"

Notepad::Notepad(const std::vector<Player>& players, const std::vector<std::vector<int>>& alibis,
    const GameBoard& game_board)
{
    for(int i = 0; i < players.size(); ++i)
    {
        auto& player = players[i];
        auto& alibi = alibis[i];

        std::vector<Note> player_notes;
        for(auto room_id : alibi)
        {
            if(room_id != -1)
                player_notes.push_back({game_board.get_room(room_id).get_name(), true});
            else
                player_notes.push_back({"", false});
        }

        notes.insert({player.get_nickname(), player_notes});
    }
}

const std::vector<Player> Notepad::get_players() const
{
    std::vector<Player> players;
    players.reserve(notes.size());
    for(auto& entry : notes)
        players.push_back(entry.first);

    return players;
}

const std::vector<Notepad::Note>& Notepad::get_notes(const Player& player) const
{
    return notes.at(player);
}

