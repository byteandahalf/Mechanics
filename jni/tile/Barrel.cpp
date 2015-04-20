#include "Barrel.h"
#include "entity/BarrelEntity.h"
#include "Utils.h"

Barrel::Barrel(int id) : EntityTile(id, "log", &Material::wood)
{
	this->setDescriptionId("barrel");
	this->setDestroyTime(0.5);
	this->tileEntityType = TileEntityType::Barrel;
}

int Barrel::getColor(TileSource*, int, int, int)
{
	return 0xff763500;
}

int Barrel::getColor(int idk)
{
	return 0xff763500;
}

int Barrel::getResource(int idk, Random* rand)
{
	return BARREL_ID;
}

int Barrel::getResourceCount(Random* rand)
{
	return 1;
}

void Barrel::onRemove(TileSource* ts, int x, int y, int z)
{
	BarrelEntity* container = (BarrelEntity*)ts->getTileEntity(x, y, z);
	if(container == NULL)
		return;

	while(container->itemCount > 0 && container->itemInstance->getId() != 0)
	{
		if(container->itemCount >= container->itemInstance->getMaxStackSize()) {
			ItemInstance* ii = new ItemInstance(container->itemInstance->getId(), container->itemInstance->getMaxStackSize(), container->itemInstance->auxValue);
			dropItem(ts, ii, x, y, z);
			container->itemCount -= container->itemInstance->getMaxStackSize();
		} else {
			ItemInstance* ii = new ItemInstance(container->itemInstance->getId(), container->itemCount, container->itemInstance->auxValue);
			dropItem(ts, ii, x, y, z);
			container->itemCount -= container->itemCount;
		}
	}
}

bool Barrel::use(Player* player, int x, int y, int z)
{
	BarrelEntity* container = (BarrelEntity*)player->region->getTileEntity(x, y, z);
	if(container == NULL)
		return false;

	ItemInstance* instance = player->getSelectedItem();
	if(container->itemInstance->isNull() && instance != NULL && instance->isStackable()) {
		container->itemInstance = ItemInstance::clone(instance);
		container->maxItems =  container->itemInstance->getMaxStackSize() * 64;
		container->itemCount += instance->count;

		player->inventory->clearSlot(player->inventory->selected);
		
	} else if((instance == NULL || instance->getId() != container->itemInstance->getId()) && container->itemCount > 0) {
		Inventory* inv = player->inventory;
		int slot = getSlotIfExistItemAndNotFull(inv, container->itemInstance->getId(), container->itemInstance->auxValue, container->itemInstance->getMaxStackSize());
		if(slot >= 0) { //If player have stack of the item incomplete
			inv->getItem(slot)->count += 1;
		} else if(inv->getFreeSlot() > 0) { // if player have some space free
			inv->addItem(new ItemInstance(container->itemInstance->getId(), 1, container->itemInstance->auxValue));
		} else { // Drop Item to the floor.
			dropItem(player->region, new ItemInstance(container->itemInstance->getId(), 1, container->itemInstance->auxValue), x, y, z);
		}
		container->itemCount -= 1;
	} else if(instance != NULL && 
			 (instance->auxValue == container->itemInstance->auxValue)   && 
			 (container->itemCount > 0) 				   && 
			 (container->itemCount < container->maxItems) &&
			 (instance->getId() == container->itemInstance->getId()))   {

		if((container->itemCount + instance->count) > container->maxItems)
		{
			int i = (container->itemCount + instance->count) - container->maxItems;
			instance->count = i;
			container->itemCount = container->maxItems;
		}else {
			container->itemCount += instance->count;
			player->inventory->clearSlot(player->inventory->selected);
		}
	}

	if(container->itemCount <= 0)
		container->clear();

	if(instance == NULL)
		return false;

	return true;
}



void Barrel::attack(Player* player, int x, int y, int z)
{
	BarrelEntity* container = (BarrelEntity*)player->region->getTileEntity(x, y, z);
	if(container == NULL || container->itemInstance->getId() <= 0)
		return;

	Inventory* inv = player->inventory;
	if(container->itemCount >= container->itemInstance->getMaxStackSize()) {
		ItemInstance* ii = new ItemInstance(container->itemInstance->getId(), container->itemInstance->getMaxStackSize(), container->itemInstance->auxValue);
		int slot = getSlotIfExistItemAndNotFull(inv, container->itemInstance->getId(), container->itemInstance->auxValue, container->itemInstance->getMaxStackSize());
		if(slot >= 0) {
			ItemInstance* item = inv->getItem(slot);
			int i = container->itemInstance->getMaxStackSize() - item->count;
			item->count += i;
			container->itemCount -= i;
			delete ii;
		} else if(inv->getFreeSlot() > 0) {
			inv->addItem(ii);
			container->itemCount -= container->itemInstance->getMaxStackSize();
		} else {
			dropItem(player->region, ii, x, y, z);
			container->itemCount -= container->itemInstance->getMaxStackSize();
		}
	} else if(container->itemCount > 0) {
		ItemInstance* ii = new ItemInstance(container->itemInstance->getId(), container->itemCount, container->itemInstance->auxValue);
		int slot = getSlotIfExistItemAndNotFull(inv, container->itemInstance->getId(), container->itemInstance->auxValue, container->itemInstance->getMaxStackSize());
		if(slot >= 0) {
			ItemInstance* item = inv->getItem(slot);
			int i = container->itemCount + item->count;
			if(i > container->itemInstance->getMaxStackSize()) {
				item->count = container->itemInstance->getMaxStackSize();
				i -= container->itemInstance->getMaxStackSize();
				if(inv->getFreeSlot() > 0) {
					inv->addItem(new ItemInstance(container->itemInstance->getId(), i, container->itemInstance->auxValue));
				} else {
					dropItem(player->region, new ItemInstance(container->itemInstance->getId(), i, container->itemInstance->auxValue), x, y, z);
				}
			} else {
				item->count += container->itemCount;
			}
			delete ii;
		} else if(inv->getFreeSlot() > 0) {
			inv->addItem(ii);
		} else {
			dropItem(player->region, ii, x, y, z);
		}
		container->clear();
	}
}
