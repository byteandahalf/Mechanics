#ifndef ENTITY_TILE_H_
#define ENTITY_TILE_H_

#include <mcpe.h>

class TileEntity;

class EntityTile : public Tile
{
	
public:
	EntityTile(int, const Material*);
	EntityTile(int, std::string const&, const Material*);
	~EntityTile();

	virtual void triggerEvent(TileSource *,int,int,int,int,int);
	virtual void neighborChanged(TileSource *,int,int,int,int,int,int);
	virtual TileEntity* newTileEntity(TilePos const&);
};


#endif
