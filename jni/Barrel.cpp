#include "Barrel.h"

Barrel::Barrel(int id) : Tile(id, "cobblestone", &Material::wood) {
	this->itemID = 0;
	this->setDestroyTime(0.5);
}

void Barrel::use(Player* player, int x, int y, int z)
{
	ItemInstance* instance = Player_getCarriedItem(player);
	int id;
	if(instance == NULL) {
		id = 0;
	} else {
		id = ItemInstance_getID(instance);
	}

	Inventory* inv = getInventory(player);
	if(this->itemID == 0 && ItemInstance_isStackable(instance)) {
		this->itemID = id;
		this->maxStackSize = ItemInstance_getMaxStackSize(instance);
		this->maxItems =  this->maxStackSize * 36;
		this->itemsCount = 0;
		this->itemDamage = instance->damage;
	} else if (this->itemID != id) {
		int slott = getSlotIfExistItemAndNotFull(inv, this->itemID, this->itemDamage, this->maxStackSize);
		if(slott >= 0) { // If there is a satck of the item
			ItemInstance* ret = FillingContainer_getItem(inv, slott);
			ret->count += 1;
			FillingContainer_replaceSlot(inv, slott, ret);
			ret = NULL;
		} else if(FillingContainer_getFreeSlot(inv) > 0) { // If the player have space add the item to the player
			ItemInstance* ii = create_ItemInstance(this->itemID, 1, this->itemDamage);
			FillingContainer_addItem(inv, ii);
			ii = NULL;

		} else { // If player full drop it in the floor
			//TODO: Drop item if player is full
			//Level* level = getLevel(player);
			//ItemInstance* ii = create_ItemInstance(this->itemID, 1, this->itemDamage);
		}
		this->itemsCount -= 1;
		return;
	}

	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Put item inside the barrel: ItemInstance(%d, %d, %d) Barrel(%d, %d, %d, %d)", id, instance->damage, instance->count, this->itemID, this->itemsCount, this->itemDamage, this->maxItems);

	int slot = ((int*) inv)[10]; // From BlockLauncher Sources
	if(this->itemsCount + instance->count > this->maxItems) {
		int i = (this->itemsCount + instance->count) - this->maxItems;
		this->itemsCount += (instance->count - i);

		ItemInstance* ii = create_ItemInstance(this->itemID, i, this->itemDamage);
		FillingContainer_setItem(inv, slot, ii);
		ii = NULL;
	} else {
		this->itemsCount += instance->count;
		FillingContainer_clearSlot(inv, slot);
	}

	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Barrel: ItemInstance(%d, %d, %d) Barrel(%d, %d, %d, %d)", id, instance->damage, instance->count, this->itemID, this->itemsCount, this->itemDamage, this->maxItems);
}



void Barrel::attack(Player* player, int x, int y, int z)
{
	Inventory* inv = getInventory(player);
	if(this->itemsCount >= this->maxStackSize) {
		if(FillingContainer_getFreeSlot(inv) > 0) {
			this->itemsCount -= this->maxStackSize;
			ItemInstance* ii = create_ItemInstance(this->itemID, this->maxStackSize, this->itemDamage);
			FillingContainer_addItem(inv, ii);
			ii = NULL;
		} else {
			//TODO: Drop item to the floor
		}
	} else {
		if(FillingContainer_getFreeSlot(inv) > 0) {
			ItemInstance* ii = create_ItemInstance(this->itemID, this->itemsCount, this->itemDamage);
			FillingContainer_addItem(inv, ii);
			this->itemsCount = 0;
			ii = NULL;
		} else {
			//TODO: Drop item to the floor
		}
	}
}