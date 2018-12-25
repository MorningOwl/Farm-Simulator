#pragma once
#include "Player.h"

class Crop
{
	public:
		int y, x;
		bool is_new = true;
		bool is_watered = false;
		bool is_grown = false;
		bool is_harvested = false;
		int days_watered;

		void operator=(Player player);
		bool operator==(Player player);
};