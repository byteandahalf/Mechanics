#ifndef UTILS_H_
#define UTILS_H_

#include <mcpe.h>
#include <sstream>
#include <stdlib.h>
#include <stdint.h>
#include <android/log.h>
#include <dlfcn.h>
#include <map>
#include "Container.h"

#define DEBUG 1
#define LOG_TAG "StorageEX"
#define PLAYER_INVENTORY_OFFSET 3212
#define PLAYER_LEVEL_OFFSET 68 // From Entity::playSound(std::string const&,float,float)
#define LEVEL_TILE_SOURCE_OFFSET 2976
#define ENTITY_ISPICKABLE 41
#define ENTITY_PLAYERTOUCH 38

#define TAG_BYTE 1
#define TAG_INT 3
#define TAG_STRING 8
#define TAG_COMPOUND 10

class Level;
class LevelData;
class Font;
class MaterialPtr;
class TileSource;

class Tag {
public:
  void** vtable;
  int errorState;
  std::string name;
  char filler[8];
};

class CompoundTag : public Tag{
public:
    std::map<std::string, Tag*> tags;
};

class IntTag : public Tag {
public:
	int value;
};

class ByteTag : public Tag{
public:
	char value;
};

class StringTag : public Tag {
public:
	std::string value; //12?
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

extern void (*Tag_Tag)(Tag*, std::string const&);
extern Tag* (*Tag_newTag)(Tag*, char, std::string const&);

extern CompoundTag* (*CompoundTag_copy)(CompoundTag*);
extern int (*CompoundTag_getID) (CompoundTag*);
extern void (*CompoundTag_put)(CompoundTag*, std::string const&, Tag*);
extern void (*CompoundTag_putInt)(CompoundTag*, std::string const&, int);
extern void (*CompoundTag_putByte)(CompoundTag*, std::string const&, char);
extern void (*CompoundTag_putString)(CompoundTag*, std::string const&, std::string const&);

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

extern void (*Font_draw)(Font*, std::string const&, float, float, Color*);

extern LevelData* (*Level_getLevelData)(Level*);
extern void (*LevelData_setTagData)(LevelData*, CompoundTag*);
extern Tag* (*LevelData_getTagData)(LevelData*, CompoundTag*);

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

void saveContainers(Level*, std::map<std::string, Container*>);

void bl_dumpVtable(void** vtable, size_t size);

#endif