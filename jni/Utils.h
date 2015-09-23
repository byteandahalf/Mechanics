#ifndef UTILS_H_
#define UTILS_H_

#include <sstream>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
#include <map>


#include "CTypes.h"
#include "MCPE/world/level/TileSource.h"
#include "mcpe/world/level/Level.h"
#include "mcpe/world/item/ItemInstance.h"
#include "mcpe/world/entity/player/Inventory.h"

#define DEBUG 0


int getSlotIfExistItemAndNotFull(Inventory* inv, int id, int damage, int maxStack);
void dropItem(TileSource*, ItemInstance*, float, float, float);


void bl_dumpVtable(void** vtable, size_t size);


#endif
