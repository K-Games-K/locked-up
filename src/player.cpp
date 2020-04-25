#include "player.hpp"

Player::Player(const std::string& nickname, int x, int y)
        : nickname(nickname), position(x, y)
{}

Player::Player(const std::string& nickname, sf::Vector2i position)
        : nickname(nickname), position(position)
{}

void Player::set_nickname(const std::string& nickname)
{
    this->nickname = nickname;
}

std::string Player::get_nickname() const
{
    return nickname;
}

void Player::set_position(int x, int y)
{
    this->position = {x, y};
}

void Player::set_position(sf::Vector2i position)
{
    this->position = position;
}

sf::Vector2i Player::get_position() const
{
    return position;
}

void Player::move(int x, int y)
{
    this->position += {x, y};
}

bool Player::operator==(const Player& other) const
{
    return nickname == other.nickname;
}
