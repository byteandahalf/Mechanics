#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <string>
#include <time.h>
#include <stdlib.h>

#include "Substrate.h"
#include "Barrel.h"
#include "BarrelEntity.h"
#include "mcpe/item/TileItem.h"
#include "mcpe/tile/entity/SignTileEntity.h"
#include "mcpe/tile/entity/ChestTileEntity.h"
#include "mcpe/tile/entity/FurnaceTileEntity.h"
#include "mcpe/tile/entity/MobSpawnerTileEntity.h"
#include "mcpe/tile/entity/NetherReactorTileEntity.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "inusualz", __VA_ARGS__))

static void (*_Item$initItems)();
static void Item$initItems()
{
	_Item$initItems();
}

static void (*_Tile$initTiles)();
static void Tile$initTiles()
{
	_Tile$initTiles();
	Tile::tiles[205] = new Barrel(205);

	TileItem* barrelItem = new TileItem(205 - 0x100);
}

static void (*_TileEntity$initEntities)();
static void TileEntity$initTileEntities()
{
	_TileEntity$initEntities();
	TileEntity::setId(TileEntityType::Barrel, "Barrel");
}

static std::string (*_I18n$get)(std::string const&);
static std::string I18n$get(std::string const& key)
{
    if(key == "tile.barrel.name") return "Barrel";
	return _I18n$get(key);
}

static TileEntity* (*_TileEntityFactory$createTileEntity)(TileEntityType, const TilePos&);
static TileEntity* TileEntityFactory$createTileEntity(TileEntityType type, const TilePos& pos)
{
	if(type == TileEntityType::Barrel) return new BarrelEntity(pos);
	else if(type == TileEntityType::Furnace) return new FurnaceTileEntity(pos);
	else if(type == TileEntityType::Chest) return new ChestTileEntity(pos);
	else if(type == TileEntityType::Sign) return new SignTileEntity(pos);
	else if(type == TileEntityType::NetherReactor) return new NetherReactorTileEntity(pos);
	else if(type == TileEntityType::MobSpawner) return new MobSpawnerTileEntity(pos);
	return _TileEntityFactory$createTileEntity(type, pos);
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    MSHookFunction((void*)&Item::initItems, (void*)&Item$initItems, (void**)&_Item$initItems);
    MSHookFunction((void*)&Tile::initTiles, (void*)&Tile$initTiles, (void**)&_Tile$initTiles);
    MSHookFunction((void*)&TileEntity::initTileEntities, (void*)&TileEntity$initTileEntities, (void**)&_TileEntity$initEntities);

    void* tileEntityFactory$createTileEntity = dlsym(RTLD_DEFAULT, "_ZN17TileEntityFactory16createTileEntityE14TileEntityTypeRK7TilePos");
    MSHookFunction(tileEntityFactory$createTileEntity, (void*)&TileEntityFactory$createTileEntity, (void**)&_TileEntityFactory$createTileEntity);

    void* I18n$get_ = dlsym(RTLD_DEFAULT, "_ZN4I18n3getERKSs");
    MSHookFunction(I18n$get_, (void*) &I18n$get, (void**) &_I18n$get);



	return JNI_VERSION_1_2;
}
