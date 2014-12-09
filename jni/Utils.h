#ifndef UTILS_H_
#define UTILS_H_

#include <mcpe.h>
#include <stdlib.h>
#include <stdint.h>
#include <android/log.h>


#define LOG_TAG "StorageEX"
#define PLAYER_INVENTORY_OFFSET 3212
#define PLAYER_LEVEL_OFFSET 68 // From Entity::playSound(std::string const&,float,float)
#define LEVEL_TILE_SOURCE_OFFSET 2976

class Level;
class Font;
class MaterialPtr;
class TileSource;

class Entity {};
class ItemEntity : public Entity {};

class FillingContainer {};

class Inventory : public FillingContainer {};
	
class ItemInstance
{
public:
	int count; //0
	int damage; //4
	Item* item;//8
	void* block; //12
	bool wtf; //16
	bool wtf2; //17
	bool wtf3; //18
	bool wtf4; //19
};

class Vec3{
public:
	float x;
	float y;
	float z;
};

class TilePos{
public:
	int x;
	int y;
	int z;
};

class Color
{
public:
	float r;
	float g;
	float b;
	float a;
};

extern void (*FillingContainer_replaceSlot)(FillingContainer*, int, ItemInstance*);
extern int (*FillingContainer_getSlotWithItem)(FillingContainer*, ItemInstance*);
extern int (*FillingContainer_getFreeSlot)(FillingContainer*);
extern int (*FillingContainer_clearSlot)(FillingContainer*, int);
extern ItemInstance* (*FillingContainer_getItem)(FillingContainer*, int);
extern void (*FillingContainer_addItem)(FillingContainer*, ItemInstance*);
extern void (*FillingContainer_setItem)(FillingContainer*, int, ItemInstance*);

extern int (*Inventory_getSelected)(Inventory*);

extern ItemInstance* (*Player_getCarriedItem)(Player*);

extern void (*ItemInstance_setID)(ItemInstance*, int);
extern int (*ItemInstance_getID)(ItemInstance*);
extern int (*ItemInstance_getMaxStackSize)(ItemInstance*);
extern bool (*ItemInstance_isStackable)(ItemInstance*);

extern Font* g_font;
extern Color g_color;
extern MaterialPtr* g_material;

extern void (*Font_drawCached_real)(Font*, std::string const&, float, float, Color const&, bool, MaterialPtr*);

extern void (*Level_addEntity)(Level*, Entity*);

extern void (*Entity_setPos)(Entity*, float, float, float);
extern void (*Entity_spawnAtLocation)(Entity*, ItemInstance*, float);
extern Entity* (*Entity_Factory)(int, TileSource*);
extern int (*ItemEntity_getEntityTypeId)(ItemEntity*);

ItemInstance* getSlot(Player* player, int slot);
Inventory* getInventory(Player* player);
Level* getLevel(Player*);
TileSource* getTileSource(Level* level);
ItemInstance* create_ItemInstance(int id, int count, int damage);
int getSlotIfExistItemAndNotFull(Inventory* inv, int id, int damage, int maxStack);
void dropItem(Level*, ItemInstance*, float, float, float);


#endif