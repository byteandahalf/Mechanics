#pragma once

#include "MCPE/world/level/block/EntityBlock.h"

class BarrelBlock : public EntityBlock
{
public:
	BarrelBlock(int blockId);

	virtual void onRemove(BlockSource& blockSource, const BlockPos&  pos);
	virtual bool use(Player&, const BlockPos&);
	virtual void attack(Player*, const BlockPos&);
};