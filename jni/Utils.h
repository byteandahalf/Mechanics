#ifndef UTILS_H_
#define UTILS_H_

#include <mcpe.h>
#include <stdlib.h>
#include <stdint.h>
#include <android/log.h>


#define LOG_TAG "StorageEX"
#define PLAYER_INVENTORY_OFFSET 3212
#define PLAYER_LEVEL_OFFSET 68 // From Entity::playSound(std::string const&,float,float)

class Level;

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


ItemInstance* getSlot(Player* player, int slot);
Inventory* getInventory(Player* player);
Level* getLevel(Player*);
ItemInstance* create_ItemInstance(int id, int count, int damage);
int getSlotIfExistItemAndNotFull(Inventory* inv, int id, int damage, int maxStack);

#endif