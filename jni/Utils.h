#ifndef UTILS_H_
#define UTILS_H_

#include <sstream>
#include <stdlib.h>
#include <stdint.h>
#include <android/log.h>
#include <dlfcn.h>
#include <map>

#include "mcpe/level/Level.h"
#include "mcpe/item/ItemInstance.h"
#include "mcpe/entity/player/Inventory.h"

#define DEBUG 0
#define LOG_TAG "StorageEX"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))


int getSlotIfExistItemAndNotFull(Inventory* inv, int id, int damage, int maxStack);
void dropItem(TileSource*, ItemInstance*, float, float, float);


void bl_dumpVtable(void** vtable, size_t size);

#endif
