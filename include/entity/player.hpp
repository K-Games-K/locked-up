#pragma once

#include <string>

class Player
{
private:
	std::string nickname;
	int x;
	int y;


public:
	Player(const std::string& nickname, int x, int y);

	int get_x() const;
	int get_y() const;

	void set_position(int m, int y);
};
