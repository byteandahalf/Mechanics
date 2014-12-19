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


//236