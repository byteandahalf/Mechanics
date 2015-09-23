#pragma once

#include "MCPE/world/level/tile/entity/TileEntity.h"

#define BARREL_STRING_ID "Barrel"

class BarrelEntity : public TileEntity
{
public:

	bool isLocked;

	ItemInstance* itemInstance;

	int itemCount;
	int maxItems;

	BarrelEntity(const TilePos&);

	virtual void load(CompoundTag*);
	virtual bool save(CompoundTag*);

	void clear();
};