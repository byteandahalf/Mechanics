#pragma once

#include "MCPE/world/item/TileItem.h"
#include "MCPE/world/level/tile/EntityTile.h"

class BarrelTile : public EntityTile
{
public:
	static const int BARREL_ID = 205;
	
	BarrelTile();

	virtual bool use(Player*, int, int, int);
	virtual void attack(Player*, int, int, int);
	virtual void onRemove(TileSource*, int, int, int);
	virtual int getResource(int, Random*); // 32
};