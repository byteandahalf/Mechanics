#include "Utils.h"

ItemInstance* getSlot(Player* player, int slot)
{
	void* invPtr = *((void**) (((intptr_t) player) + PLAYER_INVENTORY_OFFSET));
	return FillingContainer_getItem((Inventory*)invPtr, slot);
}

Inventory* getInventory(Player* player)
{
	void* invPtr = *((void**) (((intptr_t) player) + PLAYER_INVENTORY_OFFSET));
	return (Inventory*) invPtr;
}

ItemInstance* create_ItemInstance(int id, int count, int damage)
{
	ItemInstance* instance = new ItemInstance();
	instance->count = count;
	instance->damage = damage;
	ItemInstance_setID(instance, id);
	return instance;
}

Level* getLevel(Player* player)
{
	void* levelPtr = *((void**) (((intptr_t) player) + PLAYER_LEVEL_OFFSET));
	return (Level*) levelPtr;
}


int getSlotIfExistItemAndNotFull(Inventory* inv, int id, int damage, int maxStack)
{
	for (int i = 0; i < 36; i++)
	{
		ItemInstance* temp = FillingContainer_getItem(inv, i);
		if(temp != NULL && ItemInstance_getID(temp) == id && temp->damage == damage && temp->count < maxStack)
			return i;
	}
	return -1;
}

TileSource* getTileSource(Level* level)
{
	void* tileSource = *((void**) (((intptr_t) level) + LEVEL_TILE_SOURCE_OFFSET));
	return (TileSource*) tileSource;
}

void dropItem(Level* level, ItemInstance* instance, float x, float y, float z)
{
	TileSource* tileSource = getTileSource(level);
	ItemEntity* ie = (ItemEntity*)Entity_Factory(64, tileSource);
	Entity_setPos(ie, x, y, z);
	Entity_spawnAtLocation(ie, instance, 0);
}

std::string getIdentifier(Level* level, int x, int y, int z)
{
	LevelData* levelData = Level_getLevelData(level);
	std::stringstream temp;
	temp.str(LevelData_getLevelName(levelData).c_str());
	temp << x;
	temp << y;
	temp << z;
	return temp.str();
}