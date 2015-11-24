#pragma once

#include "MCPE/inventory/FillingContainer.h"
#include "MCPE/world/item/ItemInstance.h"
#include "MCPE/world/level/block/entity/BlockEntity.h"

#define GRINDER_STRING_ID "Grinder"

class GrinderEntity : public BlockEntity
{
public:
	ItemInstance* inputItem;
	ItemInstance* outputItem;
	int tickCount;

public:
	GrinderEntity(const BlockPos& pos);
	virtual ~GrinderEntity();

	virtual void tick(BlockSource& blockSource);

	virtual void load(CompoundTag& tag);
	virtual bool save(CompoundTag& tag);

};

