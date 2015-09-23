#pragma once

#include "MCPE/world/level/tile/entity/TileEntity.h"

class GrinderEntity : public TileEntity
{
public:
	//u8 progress;
	//Item  array;
	GrinderEntity(TilePos const&);
	virtual bool save(CompoundTag*);
	virtual void tick(TileSource*);
};