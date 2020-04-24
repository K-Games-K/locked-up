#include "player.hpp"

Player::Player(const std::string& nickname, int x, int y)
	: nickname(nickname), x(x), y(y)
{}

void Player::set_nickname(const std::string& nickname)
{
    this->nickname = nickname;
}

std::string Player::get_nickname() const
{
    return nickname;
}

int Player::get_x() const
{
	return this->x;
}

int Player::get_y() const
{
	return this->y;
}

void Player::set_position(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Player::move(int x, int y)
{
    this->x += x;
    this->y += y;
}

bool Player::operator==(const Player& other) const
{
    return nickname == other.nickname;
}
