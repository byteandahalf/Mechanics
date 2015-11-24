#pragma once

#include "MCPE/world/level/block/EntityBlock.h"

class GrinderBlock : public EntityBlock
{
public:
	GrinderBlock(int blockId);

	virtual bool use(Player&, const BlockPos&);
};