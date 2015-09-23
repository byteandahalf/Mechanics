#pragma once

#include "MCPE/world/item/TileItem.h"
#include "MCPE/world/level/tile/EntityTile.h"

#define GRINDER_ID 200

class GrinderTile : public EntityTile
{
public:
	GrinderTile();
	
	virtual int getResource(Random*, int, int);
	virtual bool use(Player*, int, int, int);
	virtual void tick(TileSource*, int, int, int, Random*);
};