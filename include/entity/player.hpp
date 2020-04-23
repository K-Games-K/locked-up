#pragma once

class Player
{
private:

	int id;
	int m;
	int n;


public:

	Player(int uniq_iq);

	int getN();
	int getM();

	void sePosition(int m, int n);
};