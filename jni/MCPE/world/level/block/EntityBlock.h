#pragma once

#include "Block.h"
#include "entity/BlockEntity.h"

// Size : 140
// Source: https://github.com/KsyMC/MCPE-addons/blob/master/minecraftpe/block/EntityBlock.h 
class EntityBlock : public Block
{
public:
	EntityBlock(std::string const&, int, Material const&);
	EntityBlock(std::string const&, int, std::string const&, Material const&);

	virtual ~EntityBlock();
	virtual void neighborChanged(BlockSource&, BlockPos const&, BlockPos const&);
	virtual void triggerEvent(BlockSource&, BlockPos const&, int, int);

	std::unique_ptr<BlockEntity> newBlockEntity(BlockPos const&);
};