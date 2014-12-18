#ifndef ENTITY_TILE_H_
#define ENTITY_TILE_H_

#include <mcpe.h>
#include <dlfcn.h>
#include <android/log.h>

class TileEntity;

class EntityTile : public Tile
{
public:
	char filler[100];
public:
	EntityTile(int, Material*);
	EntityTile(int, std::string const&, Material*);
	//virtual ~EntityTile();

	virtual void triggerEvent(TileSource *,int,int,int,int,int);
	virtual void neighborChanged(TileSource *,int,int,int,int,int,int);
	virtual TileEntity* newTileEntity(TilePos const&);

	static void initEntityTile();
};

static TileEntity* (*_EntityTile$newTileEntity)(EntityTile*, TilePos const&);
static void (*_EntityTile$neighborChanged)(EntityTile*, TileSource*, int, int, int, int, int, int);
static void (*_EntityTile$triggerEvent)(EntityTile*, TileSource *,int, int, int, int, int);

//static void (*_EntityTile$EntityTile)(EntityTile*, int, std::string const&, Material*);
//static void (*_EntityTile$EntityTile)(EntityTile*, int, Material*);


#endif