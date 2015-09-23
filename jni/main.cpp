#include <dlfcn.h>
#include <jni.h>
#include <stdlib.h>
#include <Substrate.h>

#include "CTypes.h"

#include "MCPE/locale/I18n.h"

#include "Mechanics/tile/BarrelTile.h"
#include "Mechanics/tile/entity/BarrelEntity.h"
#include "Mechanics/tile/GrinderTile.h"
#include "Mechanics/tile/entity/GrinderEntity.h"

static BarrelTile* g_barrel = nullptr;
static GrinderTile* g_grinder = nullptr;

static void (*_Tile$initTiles)();
static void Tile$initTiles() // Init all the tiles
{
	_Tile$initTiles();

	//Register new blocks
	g_grinder = new GrinderTile();
	g_barrel = new BarrelTile();
}

static void (*_TileEntity$initTileEntities)();
static void TileEntity$initTileEntities()
{
	_TileEntity$initTileEntities();

	//Register our tile entities.
	TileEntity::setId(TileEntityType::Grinder, "Grinder");
	TileEntity::setId(TileEntityType::Barrel, "Barrel");
}


static std::unique_ptr<TileEntity> (*_TileEntityFactory$createTileEntity)(TileEntityType,  TilePos const&);
static std::unique_ptr<TileEntity> TileEntityFactory$createTileEntity(TileEntityType type, TilePos const& pos)
{
	if(type == TileEntityType::Grinder) return std::unique_ptr<TileEntity>(new GrinderEntity(pos));
	if(type == TileEntityType::Barrel) return std::unique_ptr<TileEntity>(new BarrelEntity(pos));
	return _TileEntityFactory$createTileEntity(type, pos);
}



static std::string (*_I18n$get)(std::string const&, std::vector<std::string> const&);
static std::string I18n$get(std::string const& key, std::vector<std::string> const& a) 
{
	if(key == "tile.grinder.name") return "Grinder";
	if(key == "tile.barrel.name") return "Barrel";
	return _I18n$get(key, a);
}


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) 
{
	
	MSHookFunction((void*) &I18n::get, (void*) &I18n$get, (void**) &_I18n$get);
	MSHookFunction((void*) &Tile::initTiles, (void*) &Tile$initTiles, (void**) &_Tile$initTiles); 
	MSHookFunction((void*) &TileEntity::initTileEntities, (void*) &TileEntity$initTileEntities, (void**) &_TileEntity$initTileEntities);
	MSHookFunction((void*) &TileEntityFactory::createTileEntity, (void*) &TileEntityFactory$createTileEntity, (void**) &_TileEntityFactory$createTileEntity);
	return JNI_VERSION_1_2;
}
