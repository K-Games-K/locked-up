#pragma once

#include <string>

class Player
{
private:
	std::string nickname;
	int x;
	int y;

public:
	Player(const std::string& nickname, int x = 0, int y = 0);

	void set_nickname(const std::string& nickname);

	std::string get_nickname() const;

	int get_x() const;
	int get_y() const;

	void set_position(int x, int y);

	void move(int x, int y);

	bool operator==(const Player& other) const;
};
