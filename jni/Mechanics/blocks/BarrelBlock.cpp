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
	BarrelEntity* container = (BarrelEntity*)blockSource.getBlockEntity(pos);
	if(container == nullptr)
		return;

	while(container->itemCount > 0 && container->itemInstance->getId() != 0)
	{
		int stackSize = ((container->itemCount > container->itemInstance->getMaxStackSize()) ? container->itemInstance->getMaxStackSize() : container->itemCount);
		ItemInstance* newStack = new ItemInstance(container->itemInstance->getId(), stackSize, container->itemInstance->getAuxValue());
		//TODO: Drop item to the floor
		container->itemCount -= container->itemInstance->getMaxStackSize();
	}
	container->setChanged();
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
		LOGI("ItemInstance: %d %d %d", instance->getId(), instance->getAuxValue(), instance->count);

		//This is a workaround the ItemInstance "bug" of showing wrong count
		if(instance->count > instance->getMaxStackSize() || instance->count < 0) 
			return true;

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
		//This is a workaround the ItemInstance "bug" of showing wrong count
		if(instance->count > instance->getMaxStackSize() || instance->count < 0) 
			return true;

		if((container->itemCount + instance->count) > container->maxItems) 
		{
			instance->count = (container->itemCount + instance->count) - container->maxItems;
			container->itemCount = container->maxItems;
		} 
		else 
		{
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



void BarrelBlock::attack(Player* player, const BlockPos& pos)
{
 	BarrelEntity* container = (BarrelEntity*)player->region.getBlockEntity(pos);
 	if(container == nullptr || container->itemInstance == nullptr)
 	 		return;

	int stackSize = ((container->itemCount > container->itemInstance->getMaxStackSize()) ? container->itemInstance->getMaxStackSize() : container->itemCount);
 	Inventory* playerInventory = *(Inventory**) (((uintptr_t) player) + 0xD78); // TODO: Do the f*** header of Entity, Mob, Player.
	ItemInstance newItem(container->itemInstance->getId(), stackSize, container->itemInstance->getAuxValue());
	if(!playerInventory->add(newItem))
	{
		//TODO: Drop the item to the floor
	}
	container->itemCount -= stackSize;

	if(container->itemCount <= 0)
		container->clear();

	container->setChanged();
}