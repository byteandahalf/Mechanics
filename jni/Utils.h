#ifndef UTILS_H_
#define UTILS_H_

#include <mcpe.h>
#include <sstream>
#include <stdlib.h>
#include <stdint.h>
#include <android/log.h>
#include <dlfcn.h>
#include <map>


#define DEBUG 1
#define LOG_TAG "StorageEX"
#define PLAYER_INVENTORY_OFFSET 3212
#define PLAYER_LEVEL_OFFSET 68 // From Entity::playSound(std::string const&,float,float)
#define LEVEL_TILE_SOURCE_OFFSET 2976
#define ENTITY_ISPICKABLE 41
#define ENTITY_PLAYERTOUCH 38

class Level;
class LevelData;
class Font;
class MaterialPtr;
class TileSource;

class Tag {
  void** vtable;
  int errorState;
  std::string name;
  char filler[8];
};

typedef std::map<std::basic_string<char>, Tag*, std::less<std::basic_string<char> >, std::allocator<std::pair<const std::basic_string<char>, Tag*> > > TagMap;

class CompoundTag : public Tag{
    TagMap tags;
};

class Entity {};
class ItemEntity : public Entity {
public:
	void** vtable;
};

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
extern Color* g_color;
extern MaterialPtr* g_material;

extern void (*Font_drawCached_real)(Font*, std::string*, float, float, Color*, bool, MaterialPtr*);

extern void (*Level_addEntity)(Level*, Entity*);

extern void (*Entity_setSize)(Entity*, float, float);
extern void (*Entity_setPos)(Entity*, float, float, float);
extern void (*Entity_spawnAtLocation)(Entity*, ItemInstance*, float);
extern Entity* (*Entity_Factory)(int, TileSource*);
extern int (*ItemEntity_getEntityTypeId)(ItemEntity*);

extern void (*Font_draw)(Font*, std::string*, float, float, Color*);

extern LevelData* (*Level_getLevelData)(Level*);

extern std::string (*LevelData_getLevelName)(LevelData*);

extern Level* (*TileSource_getLevel)(TileSource*);

extern void (*ItemEntity_ItemEntity)(ItemEntity*, TileSource*, float, float, float, ItemInstance*);
ItemInstance* getSlot(Player* player, int slot);

Inventory* getInventory(Player* player);
Level* getLevel(Player*);
TileSource* getTileSource(Level* level);

ItemInstance* create_ItemInstance(int id, int count, int damage);
int getSlotIfExistItemAndNotFull(Inventory* inv, int id, int damage, int maxStack);
void dropItem(Level*, ItemInstance*, float, float, float);

std::string getIdentifier(Level*, int, int, int);

bool Entity_isPickable(Entity*);
void Entity_playerTouch(Entity* ent, Player* player);
bool Entity_checkInTile(Entity*, float, float, float);

void bl_dumpVtable(void** vtable, size_t size);

#endif