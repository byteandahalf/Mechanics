#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <stdlib.h>
#include <mcpe.h>
#include <Substrate.h>
#include <map>

#include "Barrel.h"
#include "Utils.h"

void (*FillingContainer_replaceSlot)(FillingContainer*, int, ItemInstance*);
int (*FillingContainer_clearSlot)(FillingContainer*, int);
ItemInstance* (*FillingContainer_getItem)(FillingContainer*, int);
void (*FillingContainer_addItem)(FillingContainer*, ItemInstance*);
void (*FillingContainer_setItem)(FillingContainer*, int, ItemInstance*);
int (*FillingContainer_getFreeSlot)(FillingContainer*);
int (*FillingContainer_getSlotWithItem)(FillingContainer*, ItemInstance*);

int (*Inventory_getSelected)(Inventory*);

ItemInstance* (*Player_getCarriedItem)(Player*);

void (*ItemInstance_setID)(ItemInstance*, int);
int (*ItemInstance_getID)(ItemInstance*);
int (*ItemInstance_getMaxStackSize)(ItemInstance*);
bool (*ItemInstance_isStackable)(ItemInstance*);

void (*Font_drawCached_real)(Font*, std::string const&, float, float, Color const&, bool, MaterialPtr*);

static void (*Tile_initTiles_real)();

const int barrelTileId = 201;
Font* g_font;
Color g_color;
MaterialPtr* g_material;

void Font_drawCached_hook(Font* font, std::string const& textStr, float xOffset, float yOffset, Color const& color, bool isShadow, MaterialPtr* material)
{
	g_font = font;
	g_color = color;
	g_material = material;
	Font_drawCached_real(font, textStr, xOffset, yOffset, color, isShadow, material);
}

static void Tile_initTiles_hook() {
	Tile_initTiles_real();

	Barrel* barrel = new Barrel(barrelTileId);
	Tile::tiles[barrelTileId] = barrel;
	TileItem* barrelItem = new TileItem(barrelTileId - 256);

}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

	void* fontDrawCached = dlsym(RTLD_DEFAULT, "_ZN4Font10drawCachedERKSsffRK5ColorbP11MaterialPtr");
	MSHookFunction(fontDrawCached, (void*) &Font_drawCached_hook, (void**) &Font_drawCached_real);

	Player_getCarriedItem = (ItemInstance* (*) (Player*)) dlsym(RTLD_DEFAULT, "_ZN6Player14getCarriedItemEv");

	ItemInstance_setID = (void (*) (ItemInstance*, int)) dlsym(RTLD_DEFAULT, "_ZN12ItemInstance8_setItemEi");
	ItemInstance_getID = (int (*) (ItemInstance*)) dlsym(RTLD_DEFAULT, "_ZNK12ItemInstance5getIdEv");
	ItemInstance_getMaxStackSize =(int (*) (ItemInstance*)) dlsym(RTLD_DEFAULT, "_ZNK12ItemInstance15getMaxStackSizeEv");
	ItemInstance_isStackable = (bool (*) (ItemInstance*)) dlsym(RTLD_DEFAULT, "_ZNK12ItemInstance11isStackableEv");

	FillingContainer_replaceSlot = (void (*) (FillingContainer*, int, ItemInstance*)) dlsym(RTLD_DEFAULT, "_ZN16FillingContainer11replaceSlotEiP12ItemInstance");
	FillingContainer_addItem = (void (*) (FillingContainer*, ItemInstance*)) dlsym(RTLD_DEFAULT, "_ZN16FillingContainer7addItemEP12ItemInstance");
	FillingContainer_setItem = (void (*) (FillingContainer*, int, ItemInstance*)) dlsym(RTLD_DEFAULT, "_ZN16FillingContainer7setItemEiP12ItemInstance");
	FillingContainer_getItem = (ItemInstance* (*) (FillingContainer*, int)) dlsym(RTLD_DEFAULT, "_ZN16FillingContainer7getItemEi");
	FillingContainer_clearSlot = (int (*) (FillingContainer*, int)) dlsym(RTLD_DEFAULT, "_ZN16FillingContainer9clearSlotEi");
	FillingContainer_getFreeSlot = (int (*) (FillingContainer*)) dlsym(RTLD_DEFAULT, "_ZN16FillingContainer11getFreeSlotEv");
	FillingContainer_getSlotWithItem = (int (*) (FillingContainer*, ItemInstance*)) dlsym(RTLD_DEFAULT, "_ZN16FillingContainer25getSlotWithRemainingSpaceERK12ItemInstance");

	Inventory_getSelected = (int (*) (Inventory*)) dlsym(RTLD_DEFAULT, "_ZN9Inventory11getSelectedEv");

	MSHookFunction((void*) &Tile::initTiles, (void*) &Tile_initTiles_hook, (void**) &Tile_initTiles_real);
	return JNI_VERSION_1_2;
}
