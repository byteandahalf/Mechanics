#include "Utils.h"

#include "MCPE/world/entity/item/ItemEntity.h"
#include "MCPE/world/entity/player/Player.h"

int getSlotIfExistItemAndNotFull(Inventory* inv, int id, int damage, int maxStack)
{
	for (int i = 0; i < 36; i++)
	{
		ItemInstance* temp = inv->getItem(i);
		if(temp != NULL && temp->getId() == id && temp->auxValue == damage && temp->count < maxStack)
			return i;
	}
	return -1;
}


void dropItem(TileSource* tileSource, ItemInstance* instance, float x, float y, float z)
{
	tileSource->getLevel()->addEntity(std::unique_ptr<Entity>(new ItemEntity(*tileSource, x, y, z, *instance)));
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
