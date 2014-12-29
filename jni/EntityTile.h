#ifndef ENTITY_TILE_H_
#define ENTITY_TILE_H_

#include <mcpe.h>
#include <dlfcn.h>
#include <android/log.h>

class TileEntity;

class EntityTile : public Tile
{
	
public:
	EntityTile(int, Material*);
	EntityTile(int, std::string const&, Material*);

	virtual void triggerEvent(TileSource *,int,int,int,int,int);
	virtual void neighborChanged(TileSource *,int,int,int,int,int,int);
	virtual TileEntity* newTileEntity(TilePos const&);
};


#endif