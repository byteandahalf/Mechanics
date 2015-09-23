#include "BarrelTile.h"
#include "entity/BarrelEntity.h"
#include "Utils.h"

#include "MCPE/world/item/ItemInstance.h"

BarrelTile::BarrelTile() : EntityTile(BARREL_ID, "log", &Material::wood)
{
	this->category = 1;
	this->tileEntityType = TileEntityType::Barrel;

	this->init();
	this->setNameId("barrel");
	this->setSoundType(Tile::SOUND_WOOD);
	this->setDestroyTime(0.5);

	new TileItem(BARREL_ID - 256);
}

int BarrelTile::getResource(int idk, Random* rand)
{
	return BARREL_ID;
}


void BarrelTile::onRemove(TileSource* ts, int x, int y, int z)
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
	
	container->setChanged();
}

bool BarrelTile::use(Player* player, int x, int y, int z)
{
	BarrelEntity* container = (BarrelEntity*)player->region->getTileEntity(x, y, z);
	if(container == NULL)
		return false;

	ItemInstance* instance = player->getSelectedItem();
	if(container->itemInstance == NULL && instance != NULL) {
		container->maxItems =  instance->getMaxStackSize() * 64;
		container->itemCount = instance->count;
		container->itemInstance = ItemInstance::clone(instance);

		player->inventory->clearSlot(player->inventory->getLinkedSlotForExactItem(*instance));
		LOGI("Max items: %d ItemCount: %d ItemInstance: %p", container->maxItems, container->itemCount, container->itemInstance);
		
	} else if((instance == NULL || instance->getId() != container->itemInstance->getId()) && container->itemCount > 0) {
		LOGI("Passed #1");
		Inventory* inv = player->inventory;
		int slot = getSlotIfExistItemAndNotFull(inv, container->itemInstance->getId(), container->itemInstance->auxValue, container->itemInstance->getMaxStackSize());
		if(slot >= 0) { //If player have stack of the item incomplete
			inv->getItem(slot)->count += 1;
		} else if(inv->getFreeSlot() > 0) { // if player have some space free
			LOGI("Passed #2");
			inv->addItem(new ItemInstance(container->itemInstance->getId(), 1, container->itemInstance->auxValue));
		} else { // Drop Item to the floor.
			dropItem(player->region, new ItemInstance(container->itemInstance->getId(), 1, container->itemInstance->auxValue), x, y, z);
		}
		container->itemCount -= 1;
	} else if(container->itemInstance->sameItemAndAux(instance) && (container->itemCount > 0) && (container->itemCount < container->maxItems))   {
		if((container->itemCount + instance->count) > container->maxItems) {
			instance->count = (container->itemCount + instance->count) - container->maxItems;
			container->itemCount = container->maxItems;
		} else {
			container->itemCount += instance->count;
			player->inventory->clearSlot(player->inventory->selected);
		}
	}

	container->setChanged();
	if(container->itemCount <= 0)
		container->clear();

	if(instance == nullptr)
		return false;

	return true;
}



void BarrelTile::attack(Player* player, int x, int y, int z)
{
	BarrelEntity* container = (BarrelEntity*)player->region->getTileEntity(x, y, z);
	if(container == NULL || container->itemInstance == NULL)
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
	
	container->setChanged();
}
