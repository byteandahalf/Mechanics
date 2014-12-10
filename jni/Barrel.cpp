#include "Barrel.h"

Barrel::Barrel(int id) : Tile(id, "cobblestone", &Material::wood) 
{
	this->setDestroyTime(0.5);
	this->setTicking(true);
}

void Barrel::onPlace(TileSource* ts, int x, int y, int z)
{
	Level* level = TileSource_getLevel(ts);
	std::string id = getIdentifier(level, x, y, z);
	this->containers[id] = new Container();
}

void Barrel::onRemove(TileSource* ts, int x, int y, int z)
{
	Level* level = TileSource_getLevel(ts);
	std::string id = getIdentifier(level, x, y, z);
	Container* container = this->containers.at(id);
	if(container == NULL)
		return;

	while(container->itemsCount > 0)
	{
		if(container->itemsCount >= container->maxStackSize) {
			ItemInstance* ii = create_ItemInstance(container->itemID, container->maxStackSize, container->itemDamage);
			dropItem(level, ii, x, y, z);
			container->itemsCount -= container->maxStackSize;
		} else if(container->itemsCount > 0) {
			ItemInstance* ii = create_ItemInstance(container->itemID, container->itemsCount, container->itemDamage);
			dropItem(level, ii, x, y, z);
			container->itemsCount -= container->itemsCount;
		}
	}
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
	if(container->itemID == 0 && instance != NULL && ItemInstance_isStackable(instance)) {
		container->itemID = ItemInstance_getID(instance);
		container->maxStackSize = ItemInstance_getMaxStackSize(instance);
		container->maxItems =  container->maxStackSize * 36;
		container->itemDamage = instance->damage;
		container->itemsCount += instance->count;

		Inventory* inv = getInventory(player);
		int slot = ((int*) inv)[10]; // From BlockLauncher
		FillingContainer_clearSlot(inv, slot);

#if DEBUG
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Inserted item to barrel!. Barrel Container(ID = %d, Damage = %d, MaxStackSize = %d, MaxItems = %d)", container->itemID, container->itemDamage, container->maxStackSize, container->maxItems);
#endif
	} else if(instance == NULL && container->itemsCount > 0) {
		Inventory* inv = getInventory(player);
		int slot = getSlotIfExistItemAndNotFull(inv, container->itemID, container->itemDamage, container->maxStackSize);
		if(slot >= 0) { //If player have stack of the item incomplete
			ItemInstance* retval = FillingContainer_getItem(inv, slot); 
			retval->count += 1;
			container->itemsCount -= 1;
		} else if(FillingContainer_getFreeSlot(inv) > 0) { // if player have some space free
			ItemInstance* itemInstance = create_ItemInstance(container->itemID, 1, container->itemDamage);
			FillingContainer_addItem(inv, itemInstance);
			container->itemsCount -= itemInstance->count;
		} else { // Drop Item to the floor.
			ItemInstance* itemInstance = create_ItemInstance(container->itemID, 1, container->itemDamage);
			Level* level = getLevel(player);
			dropItem(level, itemInstance, x, y, z);
			container->itemsCount -= itemInstance->count;
		}
	} else if((ItemInstance_getID(instance) == container->itemID) && 
					(instance->damage == container->itemDamage)   && 
					(container->itemsCount > 0) 				  && 
					(container->itemsCount < container->maxItems))   {

		Inventory* inv = getInventory(player);
		int slot = ((int*) inv)[10]; // From BlockLauncher

		ItemInstance* itemInstance = FillingContainer_getItem(inv, slot);
		if((container->itemsCount + itemInstance->count) > container->maxItems)
		{
			int i = (container->itemsCount + instance->count) - container->maxItems;
			itemInstance->count = i;
			container->itemsCount += (instance->count - i);
		} else {
			container->itemsCount += instance->count;
			FillingContainer_clearSlot(inv, slot);
		}
	}
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

void Barrel::tick(TileSource* ts, int x, int y, int z, Random* rand)
{
	std::string* text = new std::string("Hello World");
	Font_draw(g_font, text, 5, 5, g_color);


	glPushMatrix();

	glTranslatef(0, 2, 0);
	glScalef(15.0, 15.0, 15.0);

	Font_drawCached_real(g_font, text, 0, 0, g_color, false, g_material);
	glPopMatrix();
}