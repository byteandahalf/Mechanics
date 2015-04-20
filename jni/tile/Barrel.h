#pragma once

#include "mcpe/Tile/EntityTile.h"
#include "mcpe/entity/player/Player.h"
#include "mcpe/level/TileSource.h"
#include "mcpe/util/Random.h"

class Barrel : public EntityTile
{
public:
	static const int BARREL_ID = 205;
	
	Barrel(int id);

	virtual bool use(Player*, int, int, int);
	virtual void attack(Player*, int, int, int);
	virtual void onRemove(TileSource*, int, int, int);
	virtual int getColor(TileSource*, int, int, int);
	virtual int getColor(int);
	virtual int getResource(int, Random*); // 32
	virtual int getResourceCount(Random*); // 33
};

