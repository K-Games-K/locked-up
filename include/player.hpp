#pragma once

#include <string>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "game_board.hpp"

class Player
{
private:
    std::string nickname;
    sf::Vector2i position;

    std::vector<int> alibi;

public:
    Player(const std::string& nickname, int x = 0, int y = 0);

    Player(const std::string& nickname, sf::Vector2i position);

    void set_nickname(const std::string& nickname);

    std::string get_nickname() const;

    void set_position(int x, int y);

    void set_position(sf::Vector2i position);

    sf::Vector2i get_position() const;

    void move(int x, int y);

    void generate_alibi(const GameBoard& game_board, int crime_room, int length);

    bool operator==(const Player& other) const;
};
