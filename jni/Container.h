#ifndef CONTAINER_H_
#define CONTAINER_H_

class Container
{
public:
	int itemID;
	int itemDamage;
	int itemsCount;
	int maxItems;
	int maxStackSize;
	bool locked;

public:
	Container();
};

#endif