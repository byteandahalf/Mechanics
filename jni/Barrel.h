#ifndef BARREL_H_
#define BARREL_H_

#include <map>
#include <math.h>
#include <mcpe.h>

#include "Container.h"
#include "Utils.h"


class Barrel : public Tile
{
public:
	std::map<std::string, Container*> containers;

public:
	Barrel(int id);

	virtual bool use(Player*, int, int, int);
	virtual void attack(Player*, int, int, int);
	virtual void onPlace(TileSource*, int, int, int);
	virtual void onRemove(TileSource*, int, int, int);
	virtual int getColor(TileSource*, int, int, int);
	virtual int getColor(int);

};
#endif
