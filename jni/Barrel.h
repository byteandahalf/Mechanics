#ifndef BARREL_H_
#define BARREL_H_

#include <mcpe.h>
#include <GLES/gl.h>
#include <map>
#include <math.h>

#include "Container.h"
#include "Utils.h"

class Barrel : public Tile
{
private:
	std::map<std::string, Container*> containers;
	//ItemEntity* itemEntity;

public:
	Barrel(int id);

	virtual void use(Player*, int, int, int);
	virtual void attack(Player*, int, int, int);
	virtual void tick(TileSource*, int, int, int, Random*);
	virtual void onPlace(TileSource*, int, int, int);
	virtual void onRemove(TileSource*, int, int, int);
	virtual int getColor(TileSource*, int, int, int);
	virtual int getColor(int);

};
#endif