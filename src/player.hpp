#pragma once

#include <string>
#include <vector>
#include <set>
#include <SFML/System/Vector2.hpp>

#include "game_board.hpp"

class Player
{
private:
    std::string nickname;
    std::string avatar_name;
    sf::Vector2i position;

    std::vector<int> alibi;

public:
    explicit Player(const std::string& nickname, const std::string& avatar_name = "mr1");

    void set_nickname(const std::string& nickname);

    std::string get_nickname() const;

    void set_avatar_name(const std::string& avatar_name);

    std::string get_avatar_name() const;

    void set_position(int x, int y);

    void set_position(sf::Vector2i position);

    sf::Vector2i get_position() const;

    void move(int x, int y);

    const std::vector<int>& get_alibi() const;

    void generate_alibi(const GameBoard& game_board, int starting_room, int alibi_length);

    bool operator==(const Player& other) const;

    bool operator<(const Player& other) const;
};
