#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <stdlib.h>
#include <mcpe.h>
#include <Substrate.h>
#include <map>

#include "Barrel.h"
#include "Player.h"

std::map <std::string, std::string>* bl_I18n_strings;

const int barrelTileId = 201;
const int baconTileId = 200;

static void (*Tile_initTiles_real)();

static void Tile_initTiles_hook() {
	Tile_initTiles_real();

	Barrel* barrel = new Barrel(barrelTileId);
	Tile::tiles[barrelTileId] = barrel;
	TileItem* barrelItem = new TileItem(barrelTileId - 256);

	barrel->setDescriptionId(barrel->getName());
	(*bl_I18n_strings)["tile." + barrel->getName() +".name"] = barrel->getName();

}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	bl_I18n_strings = (std::map <std::string, std::string> *) dlsym(RTLD_DEFAULT, "_ZN4I18n8_stringsE");

	bl_Player_getCarriedItem = (ItemInstance* (*) (Player*)) dlsym(RTLD_DEFAULT, "_ZN6Player14getCarriedItemEv");
	Player_setDestPos = (void (*) (Player*, float, float, float))  dlsym(RTLD_DEFAULT, "_ZN11LocalPlayer6setPosEfff");

	MSHookFunction((void*) &Tile::initTiles, (void*) &Tile_initTiles_hook, (void**) &Tile_initTiles_real);
	return JNI_VERSION_1_2;
}
