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
	ItemEntity* retval = (ItemEntity*) ::operator new((std::size_t) 332);
	ItemEntity_ItemEntity(retval, tileSource, x, y, z, instance);
	Level_addEntity(level, retval);
}

std::string getIdentifier(Level* level, int x, int y, int z)
{
	LevelData* levelData = Level_getLevelData(level);
	if(levelData == NULL)
		return "";

	std::stringstream temp;
	temp << LevelData_getLevelName(levelData).c_str();
	temp << ":";
	temp << x;
	temp << y;
	temp << z;
	return temp.str();
}

bool Entity_isPickable(Entity* ent)
{
	return false;
}

void Entity_playerTouch(Entity* ent, Player* player)
{
	return; // Do Nothing
}

void bl_dumpVtable(void** vtable, size_t size) {
	// thanks, MrARM
	Dl_info info;
	for (int i = 0; i < (size / sizeof(void*)); i++) {
		if (!dladdr(vtable[i], &info)) {
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "BLANK // %d", i);
		} else {
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s; // %d", info.dli_sname, i);
		}
	}
}