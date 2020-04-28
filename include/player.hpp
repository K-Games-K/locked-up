#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>

class Player
{
private:
    std::string nickname;
    sf::Vector2i position;

public:
    Player(const std::string& nickname, int x = 0, int y = 0);

    Player(const std::string& nickname, sf::Vector2i position);

    void set_nickname(const std::string& nickname);

    std::string get_nickname() const;

    void set_position(int x, int y);

    void set_position(sf::Vector2i position);

    sf::Vector2i get_position() const;

    void move(int x, int y);

    bool operator==(const Player& other) const;
};
