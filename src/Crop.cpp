#include "Crop.h"


void Crop::operator=(Player player)
{
	this->x = player.x;
	this->y = player.y;
}

bool Crop::operator==(Player player)
{
	return this->x == player.x && this->y == player.y;
}