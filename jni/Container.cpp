#include "Container.h"

Container::Container(std::string levelName, int x, int y, int z)
{
	this->locked = false;
	this->itemID = 0;
	this->itemsCount = 0;
	
	this->levelName = levelName;
	this->x = x;
	this->y = y;
	this->z = z;
}