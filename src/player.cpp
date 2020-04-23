#include "player.hpp"

Player::Player(const std::string& nickname, int x, int y)
	: nickname(nickname), x(x), y(y)
{}

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
