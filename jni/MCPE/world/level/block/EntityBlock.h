#pragma once

#include "Block.h"
#include "entity/BlockEntity.h"

class EntityBlock : public Block
{
public:
	EntityBlock(const std::string&, int, const Material&);
	EntityBlock(const std::string&, int, const std::string&, const Material&);

	virtual ~EntityBlock();
	virtual void neighborChanged(BlockSource&, const BlockPos&, const BlockPos&);
	virtual void triggerEvent(BlockSource&, const BlockPos&, int, int); 

	std::unique_ptr<BlockEntity> newBlockEntity(const BlockPos&);
};