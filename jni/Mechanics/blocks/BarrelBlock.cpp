#include "BarrelBlock.h"
#include "entity/BarrelEntity.h"

#include "Common.h"
#include "MCPE/world/entity/ItemEntity.h"
#include "MCPE/world/entity/player/Player.h"
#include "MCPE/world/entity/player/Inventory.h"
#include "MCPE/world/level/Level.h"
#include "MCPE/world/material/Material.h"

BarrelBlock::BarrelBlock(int blockId) : EntityBlock("barrel", blockId, "log", Material::getMaterial(MaterialType::WOOD)) 
{
	this->setDestroyTime(0.5);
	this->setExplodeable(3.0F);
	
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
		ItemInstance newStack(container->itemInstance->getId(), stackSize, container->itemInstance->getAuxValue());
		
		blockSource.getLevel()->addEntity(std::unique_ptr<Entity>(new ItemEntity(blockSource, Vec3(pos), newStack, 1)));
		
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
			player.region.getLevel()->addEntity(std::unique_ptr<Entity>(new ItemEntity(player.region, Vec3(pos), temp, 1)));
		} 
		container->itemCount -= 1;
	} 
	else if(container->itemInstance->sameItemAndAux(instance) && (container->itemCount > 0) && (container->itemCount < container->maxItems))
	{

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

	if(instance == nullptr)
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
		player->region.getLevel()->addEntity(std::unique_ptr<Entity>(new ItemEntity(player->region, Vec3(pos), newItem, 1)));
	}
	container->itemCount -= stackSize;

	if(container->itemCount <= 0)
		container->clear();

	container->setChanged();
}