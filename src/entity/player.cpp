#include "entity/player.hpp"

Player::Player(int uniq_id)
	:m(0) , n(0) , id(uniq_id)
{}

int Player::getM()
{
	return this->m;
}

int Player::getN()
{
	return this->n;
}

void Player::sePosition(int m, int n)
{
	this->m = m;
	this->n = n;
}