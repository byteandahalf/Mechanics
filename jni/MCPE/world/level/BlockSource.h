#pragma once
#include "MCPE/CommonTypes.h"

class Block;
struct BlockPos;
class BlockEntity;
class Level;

class BlockSource {
public:
	Level* getLevel() const;
	Block* getBlock(const BlockPos&);
	FullBlock getBlockID(const BlockPos&);
	DataID getData(const BlockPos&);
	bool setBlockAndData(const BlockPos&, FullBlock, int);
	void removeBlock(const BlockPos&);
	BlockEntity* getBlockEntity(const BlockPos&);
};
