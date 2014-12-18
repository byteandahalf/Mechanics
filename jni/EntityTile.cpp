#include "EntityTile.h"


EntityTile::EntityTile(int id, Material* material) : Tile(id, material)
{
	//What to do here?
}

EntityTile::EntityTile(int id, std::string const& texture, Material* material) : Tile(id, texture, material)
{
	//what to do here?
}


void EntityTile::triggerEvent(TileSource* tileSource, int x, int y, int z, int idk, int idk2)
{
	__android_log_print(ANDROID_LOG_INFO, "StorageEx", "Triggered! #3");
	//_EntityTile$triggerEvent(this, tileSource, x, y, z, idk, idk2);
}

void EntityTile::neighborChanged(TileSource* tileSource, int x, int y, int z, int idk, int idk2, int idk3)
{
	__android_log_print(ANDROID_LOG_INFO, "StorageEx", "Triggered! #2");
	//_EntityTile$neighborChanged(this, tileSource, x, y, z, idk, idk2, idk3);
}

TileEntity* EntityTile::newTileEntity(TilePos const& pos)
{
	__android_log_print(ANDROID_LOG_INFO, "StorageEx", "Triggered!");
	return NULL;//_EntityTile$newTileEntity(this, pos);
}


void EntityTile::initEntityTile()
{
	_EntityTile$newTileEntity = (TileEntity* (*) (EntityTile*, TilePos const&)) dlsym(RTLD_DEFAULT, "_ZN10EntityTile13newTileEntityERK7TilePos");
	_EntityTile$neighborChanged = (void (*)(EntityTile*, TileSource*, int, int, int, int, int, int)) dlsym(RTLD_DEFAULT, "_ZN10EntityTile15neighborChangedEP10TileSourceiiiiii");
	_EntityTile$triggerEvent = (void (*) (EntityTile*, TileSource *,int, int, int, int, int)) dlsym(RTLD_DEFAULT, "_ZN10EntityTile12triggerEventEP10TileSourceiiiii");

	//_EntityTile$EntityTile = (void (*) (EntityTile*, int, std::string const&, Material*)) dlsym(RTLD_DEFAULT, "_ZN10EntityTileC2EiRKSsPK8Material");
	//_EntityTile$EntityTile = (void (*) (EntityTile*, int, Material*)) dlsym(RTLD_DEFAULT, "_ZN10EntityTileC2EiPK8Material");
}

//236