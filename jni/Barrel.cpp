#include "Barrel.h"

Barrel::Barrel(int id) : Tile(id, "cobblestone", &Material::wood) 
{
	this->setDestroyTime(0.5);
	this->setTicking(true);
}

void Barrel::onPlace(TileSource* ts, int x, int y, int z)
{
	Level* level = TileSource_getLevel(ts);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Getted the level pointer");
	std::string id = getIdentifier(level, x, y, z);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Getted the identifier");
	this->containers[id] = new Container();
}

void Barrel::onRemove(TileSource* ts, int x, int y, int z)
{
	Level* level = TileSource_getLevel(ts);
	std::string id = getIdentifier(level, x, y, z);
	this->containers.erase(id);
}

void Barrel::use(Player* player, int x, int y, int z)
{
	Level* level = getLevel(player);
	std::string ident = getIdentifier(level, x, y, z);
	Container* container = this->containers.at(ident);
	if(container == NULL)
		return;

	ItemInstance* instance = Player_getCarriedItem(player);
	int id;
	if(instance == NULL) {
		id = 0;
	} else {
		id = ItemInstance_getID(instance);
	}

	Inventory* inv = getInventory(player);
	if(container->itemID == 0 && id != 0 && ItemInstance_isStackable(instance)) {
		container->itemID = id;
		container->maxStackSize = ItemInstance_getMaxStackSize(instance);
		container->maxItems =  container->maxStackSize * 36;
		container->itemsCount = 0;
		container->itemDamage = instance->damage;
	} else if (container->itemID != id && container->itemsCount > 0) {
		int slott = getSlotIfExistItemAndNotFull(inv, container->itemID, container->itemDamage, container->maxStackSize);
		if(slott >= 0) { // If there is a satck of the item
			ItemInstance* ret = FillingContainer_getItem(inv, slott);
			ret->count += 1;
			FillingContainer_replaceSlot(inv, slott, ret);
			ret = NULL;
		} else if(FillingContainer_getFreeSlot(inv) > 0) { // If the player have space add the item to the player
			ItemInstance* ii = create_ItemInstance(container->itemID, 1, container->itemDamage);
			FillingContainer_addItem(inv, ii);
			ii = NULL;

		} else { // If player full drop it in the floor
			ItemInstance* ii = create_ItemInstance(container->itemID, 1, container->itemDamage);
			dropItem(level, ii, x, y, z);

			level = NULL;
			ii = NULL;
		}
		container->itemsCount -= 1;
		return;
	}

	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Put item inside the barrel: ItemInstance(%d, %d, %d) Barrel(%d, %d, %d, %d)", id, instance->damage, instance->count, container->itemID, container->itemsCount, container->itemDamage, container->maxItems);

	int slot = ((int*) inv)[10]; // From BlockLauncher Sources
	if(container->itemsCount + instance->count > container->maxItems) {
		int i = (container->itemsCount + instance->count) - container->maxItems;
		container->itemsCount += (instance->count - i);

		ItemInstance* ii = create_ItemInstance(container->itemID, i, container->itemDamage);
		FillingContainer_setItem(inv, slot, ii);
		ii = NULL;
	} else {
		container->itemsCount += instance->count;
		FillingContainer_clearSlot(inv, slot);
	}

	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Barrel: ItemInstance(%d, %d, %d) Barrel(%d, %d, %d, %d)", id, instance->damage, instance->count, container->itemID, container->itemsCount, container->itemDamage, container->maxItems);
}



void Barrel::attack(Player* player, int x, int y, int z)
{
	Level* level = getLevel(player);
	std::string id = getIdentifier(level, x, y, z);
	Container* container = this->containers.at(id);
	if(container == NULL)
		return;

	Inventory* inv = getInventory(player);
	if(container->itemsCount >= container->maxStackSize) {
		ItemInstance* ii = create_ItemInstance(container->itemID, container->maxStackSize, container->itemDamage);
		if(FillingContainer_getFreeSlot(inv) > 0) {
			FillingContainer_addItem(inv, ii);
		} else {
			dropItem(level, ii, x, y, z);
			level = NULL;
		}
		ii = NULL;
		container->itemsCount -= container->maxStackSize;
	} else if(container->itemsCount > 0) {
		ItemInstance* ii = create_ItemInstance(container->itemID, container->itemsCount, container->itemDamage);
		if(FillingContainer_getFreeSlot(inv) > 0) {
			FillingContainer_addItem(inv, ii);
		} else {
			dropItem(level, ii, x, y, z);
			level = NULL;
		}
		ii = NULL;
		container->itemsCount = 0;
	}
}

void Barrel::animateTick(TileSource* ts, int x, int y, int z, Random* rand)
{
	glPushMatrix();

	glTranslatef(x, y, z);
	glScalef(15.0, 15.0, 15.0);

	std::string* text = new std::string("Test!");
	Font_drawCached_real(g_font, text, 0, 0, g_color, false, g_material);
	glPopMatrix();
}