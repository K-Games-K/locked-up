#pragma once

#include <map>

#include "player.hpp"
#include "render/renderer.hpp"

class Notepad
{
public:
    struct Note
    {
        std::string room_name;
        bool certain;
    };

    Notepad(const std::vector<Player>& players, const std::vector<std::vector<int>>& alibis,
        const GameBoard& game_board);

    const std::vector<Player> get_players() const;

    const std::vector<Note>& get_notes(const Player& player) const;

private:
    std::map<Player, std::vector<Note>> notes;
};
