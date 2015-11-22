#pragma once

#include "MCPE/world/level/block/entity/BlockEntity.h"
#include "MCPE/world/item/ItemInstance.h"

#define BARREL_STRING_ID "Barrel"

class BarrelEntity : public BlockEntity
{
public:

	bool isLocked;

	ItemInstance* itemInstance;

	unsigned int itemCount;
	unsigned int maxItems;

	BarrelEntity(const BlockPos&);
	virtual ~BarrelEntity();

	virtual void load(CompoundTag&);
	virtual bool save(CompoundTag&);

	void clear();
};