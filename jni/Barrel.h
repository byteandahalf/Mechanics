#ifndef BARREL_H_
#define BARREL_H_

#include <mcpe.h>
#include <GLES/gl.h>
#include <map>

#include "Container.h"
#include "Utils.h"

class Barrel : public Tile
{
private:
	std::map<std::string, Container*> containers;

public:
	Barrel(int id);

	virtual void use(Player*, int, int, int);
	virtual void attack(Player*, int, int, int);
	virtual void tick(TileSource*, int, int, int, Random*);
	virtual void onPlace(TileSource*, int, int, int);
	virtual void onRemove(TileSource*, int, int, int); 

};
#endif