#pragma once

enum Map { FARM, HOUSE, STREET, SHOP };
enum Item { NONE, TURNIPSEED, WATERING_CAN, TURNIP };


class Player
{
	public:
		char name[20];
		int y, x;
		int funds = 500;

		Player(int y, int x) :y(y), x(x) {};
};