#include "BarrelBlock.h"
#include "entity/BarrelEntity.h"

#include "MCPE/world/entity/player/Player.h"
#include "MCPE/world/entity/player/Inventory.h"
#include "MCPE/world/material/Material.h"
#include "Common.h"

BarrelBlock::BarrelBlock(int blockId) : EntityBlock("barrel", blockId, "log", Material::getMaterial(MaterialType::WOOD)) 
{
	setDestroyTime(0.5);
	setExplodeable(3.0F);
	
	this->creativeCategory = 1;
	this->blockEntityType = BlockEntityType::Barrel;
}


void BarrelBlock::onRemove(BlockSource& blockSource, const BlockPos&  pos)
{
	// BarrelEntity* container = (BarrelEntity*)blockSource.getBlockEntity(x, y, z);
	// if(container == nullptr)
	// 	return;

	// while(container->itemCount > 0 && container->itemInstance->getId() != 0)
	// {
	// 	if(container->itemCount >= container->itemInstance->getMaxStackSize()) 
	// 	{
	// 		ItemInstance* ii = new ItemInstance(container->itemInstance->getId(), container->itemInstance->getMaxStackSize(), container->itemInstance->auxValue);
	// 		//dropItem(blockSource, ii, x, y, z);
	// 		container->itemCount -= container->itemInstance->getMaxStackSize();
	// 	} 
	// 	else 
	// 	{
	// 		ItemInstance* ii = new ItemInstance(container->itemInstance->getId(), container->itemCount, container->itemInstance->auxValue);
	// 		//dropItem(blockSource, ii, x, y, z);
	// 		container->itemCount -= container->itemCount;
	// 	}
	// }
	
	// container->setChanged();
}

bool BarrelBlock::use(Player& player, const BlockPos& pos)
{
	BarrelEntity* container = (BarrelEntity*)player.region.getBlockEntity(pos);
	if(container == nullptr)
		return false;

	Inventory* playerInventory = *(Inventory**) (((uintptr_t) &player) + 0xD78); // TODO: Do the f*** header of Entity, Mob, Player.
	ItemInstance* instance = player.getSelectedItem();
	if(container->itemInstance == nullptr && instance != nullptr) 
	{
		LOGI("ItemInstance Count: %d", instance->count);

	 	container->maxItems =  instance->getMaxStackSize() * 64;
		container->itemCount = instance->count;
		container->itemInstance = ItemInstance::clone(instance);

		playerInventory->clearSlot(playerInventory->getSelectedSlot());	
	} 
	else if((instance == nullptr || instance->getId() != container->itemInstance->getId()) && container->itemCount > 0)
	{
		ItemInstance temp(container->itemInstance->getId(), 1, container->itemInstance->getAuxValue());
		if(!playerInventory->add(temp)) // This function clone the ItemInstance
		{
			//If the player does not have space, drop the item to the floor.
			//TODO: Drop the item
		} 
		container->itemCount -= 1;
	} 
	else if(container->itemInstance->sameItemAndAux(instance) && (container->itemCount > 0) && (container->itemCount < container->maxItems))
	{
		if((container->itemCount + instance->count) > container->maxItems) {
			instance->count = (container->itemCount + instance->count) - container->maxItems;
			container->itemCount = container->maxItems;
		} else {
			container->itemCount += instance->count;
			playerInventory->clearSlot(playerInventory->getSelectedSlot());
		}
	}

	container->setChanged();
	if(container->itemCount <= 0)
		container->clear();

	if(instance == NULL)
		return false;

	return true;
}



void BarrelBlock::attack(Player* player, const BlockPos&)
{
// 	BarrelEntity* container = (BarrelEntity*)player.region->getBlockEntity(x, y, z);
// 	if(container == nullptr || container->itemInstance == nullptr)
// 		return;

// 	Inventory* inv = player.inventory;
// 	if(container->itemCount >= container->itemInstance->getMaxStackSize()) {
// 		ItemInstance* ii = new ItemInstance(container->itemInstance->getId(), container->itemInstance->getMaxStackSize(), container->itemInstance->auxValue);
// 		int slot = getSlotIfExistItemAndNotFull(inv, container->itemInstance->getId(), container->itemInstance->auxValue, container->itemInstance->getMaxStackSize());
// 		if(slot >= 0) {
// 			ItemInstance* item = inv->getItem(slot);
// 			int i = container->itemInstance->getMaxStackSize() - item->count;
// 			item->count += i;
// 			container->itemCount -= i;
// 			delete ii;
// 		} else if(inv->getFreeSlot() > 0) {
// 			inv->addItem(ii);
// 			container->itemCount -= container->itemInstance->getMaxStackSize();
// 		} else {
// 			dropItem(player.region, ii, x, y, z);
// 			container->itemCount -= container->itemInstance->getMaxStackSize();
// 		}
// 	} else if(container->itemCount > 0) {
// 		ItemInstance* ii = new ItemInstance(container->itemInstance->getId(), container->itemCount, container->itemInstance->auxValue);
// 		int slot = getSlotIfExistItemAndNotFull(inv, container->itemInstance->getId(), container->itemInstance->auxValue, container->itemInstance->getMaxStackSize());
// 		if(slot >= 0) {
// 			ItemInstance* item = inv->getItem(slot);
// 			int i = container->itemCount + item->count;
// 			if(i > container->itemInstance->getMaxStackSize()) {
// 				item->count = container->itemInstance->getMaxStackSize();
// 				i -= container->itemInstance->getMaxStackSize();
// 				if(inv->getFreeSlot() > 0) {
// 					inv->addItem(new ItemInstance(container->itemInstance->getId(), i, container->itemInstance->auxValue));
// 				} else {
// 					dropItem(player.region, new ItemInstance(container->itemInstance->getId(), i, container->itemInstance->auxValue), x, y, z);
// 				}
// 			} else {
// 				item->count += container->itemCount;
// 			}
// 			delete ii;
// 		} else if(inv->getFreeSlot() > 0) {
// 			inv->addItem(ii);
// 		} else {
// 			dropItem(player.region, ii, x, y, z);
// 		}
// 		container->clear();
// 	}
	
// 	container->setChanged();
}