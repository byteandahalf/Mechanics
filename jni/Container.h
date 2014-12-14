#ifndef CONTAINER_H_
#define CONTAINER_H_

#include <string>

class Container
{
public:
	int itemID;
	int itemDamage;
	int itemsCount;
	int maxItems;
	int maxStackSize;
	bool locked;

	std::string levelName;
	int x, y, z;

public:
	Container(std::string levelName, int x, int y, int z);
};

#endif