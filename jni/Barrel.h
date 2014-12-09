#ifndef BARREL_H_
#define BARREL_H_

#include <mcpe.h>
#include <GLES/gl.h>

#include "Utils.h"

class Barrel : public Tile
{
private:
	int itemID;
	int itemDamage;
	int itemsCount;
	int maxItems;
	int maxStackSize;

public:
	Barrel(int id);

	virtual void use(Player*, int, int, int);
	virtual void attack(Player*, int, int, int);
	virtual void animateTick(TileSource*, int, int, int, Random*);

};
#endif