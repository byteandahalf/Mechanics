#pragma once

#include "MCPE/world/level/block/Block.h"

class WoodenCrankBlock : public Block
{
public:
	WoodenCrankBlock(int blockId);

	virtual bool use(Player&, const BlockPos&);
};