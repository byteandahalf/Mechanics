#include "GrinderBlock.h"
#include "entity/GrinderEntity.h"

#include "Common.h"
#include "MCPE/world/entity/ItemEntity.h"
#include "MCPE/world/entity/player/Player.h"
#include "MCPE/world/entity/player/Inventory.h"
#include "MCPE/world/level/Level.h"
#include "MCPE/world/material/Material.h"

GrinderBlock::GrinderBlock(int blockId) : EntityBlock("grinder", blockId, "stone", Material::getMaterial(MaterialType::WOOD))
{
	this->setDestroyTime(0.5);
	this->setExplodeable(3.0F);
	
	this->creativeCategory = 1;
	this->blockEntityType = BlockEntityType::Grinder;

}

bool GrinderBlock::use(Player& player, const BlockPos& pos)
{
	GrinderEntity* grinderEntity = (GrinderEntity*)player.region.getBlockEntity(pos);
	if(grinderEntity == nullptr)
		return false;

	ItemInstance* itemInstance = player.getSelectedItem();
	if(itemInstance != nullptr && grinderEntity->inputItem == nullptr)
	{
		grinderEntity->inputItem = ItemInstance::clone(itemInstance);
		grinderEntity->setChanged();

		Inventory* playerInventory = *(Inventory**) (((uintptr_t) &player) + 0xD78);
		playerInventory->clearSlot(playerInventory->getSelectedSlot());
		return true;
	}
	return false;
}