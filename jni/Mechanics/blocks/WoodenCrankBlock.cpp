#include "WoodenCrankBlock.h"

#include "Common.h"
#include "entity/GrinderEntity.h"
#include "MCPE/world/entity/ItemEntity.h"
#include "MCPE/world/entity/player/Player.h"
#include "MCPE/world/entity/player/Inventory.h"
#include "MCPE/world/level/Level.h"
#include "MCPE/world/material/Material.h"

WoodenCrankBlock::WoodenCrankBlock(int blockId) : Block("wooden_crank", blockId, "log", Material::getMaterial(MaterialType::WOOD))
{
	this->setDestroyTime(0.5);
	this->setExplodeable(3.0F);
	
	this->creativeCategory = 1;
}

bool WoodenCrankBlock::use(Player& player, const BlockPos& pos)
{
	
	BlockPos newPos = {pos.x, pos.y - 1, pos.z};

	GrinderEntity* grinderEntity = (GrinderEntity*) player.region.getBlockEntity(newPos);
	if(grinderEntity == nullptr || grinderEntity->inputItem == nullptr)
		return false;

	LOGI("TickCount: %d", grinderEntity->tickCount++);
	return Block::use(player, pos);

}