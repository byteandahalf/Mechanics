#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <stdlib.h>
#include <mcpe.h>
#include <Substrate.h>
#include <map>

#include "Barrel.h"
#include "Utils.h"
#include "dlsym-weak.cpp"
#include "linker.h"

void** CompoundTag_vtable;
void** ByteTag_vtable;
void** IntTag_vtable;
void** StringTag_vtable;

void (*Tag_Tag)(Tag*, std::string const&);
Tag* (*Tag_newTag)(Tag*, char, std::string const&);

CompoundTag* (*CompoundTag_copy)(CompoundTag*); 
int (*CompoundTag_getID) (CompoundTag*);
void (*CompoundTag_put)(CompoundTag*, std::string const&, Tag*);
void (*CompoundTag_putInt)(CompoundTag*, std::string const&, int);
void (*CompoundTag_putByte)(CompoundTag*, std::string const&, char);
void (*CompoundTag_putString)(CompoundTag*, std::string const&, std::string const&);

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

void (*Font_drawCached_real)(Font*, std::string*, float, float, Color*, bool, MaterialPtr*);

void (*Level_addEntity)(Level*, Entity*);

void (*Entity_setSize)(Entity*, float, float);
void (*Entity_setPos)(Entity*, float, float, float);
void (*Entity_spawnAtLocation)(Entity*, ItemInstance*, float);
Entity* (*Entity_Factory)(int, TileSource*);
int (*ItemEntity_getEntityTypeId)(ItemEntity*);

LevelData* (*Level_getLevelData)(Level*);

std::string (*LevelData_getLevelName)(LevelData*);
void (*LevelData_setTagData)(LevelData*, CompoundTag*);
Tag* (*LevelData_getTagData)(LevelData*, CompoundTag*);

Level* (*TileSource_getLevel)(TileSource*);

void (*Font_draw)(Font*, std::string*, float, float, Color*);

void (*ItemEntity_ItemEntity)(ItemEntity*, TileSource*, float, float, float, ItemInstance*);

static void (*_Player$readAdditionalSaveData)(Player*, CompoundTag*);
static void (*_Player$addAdditionalSaveData)(Player*, CompoundTag*);

static void (*Tile_initTiles_real)();

const int barrelTileId = 201;
Font* g_font;
Color* g_color;
MaterialPtr* g_material;
Barrel* barrel;

void Font_drawCached_hook(Font* font, std::string* textStr, float xOffset, float yOffset, Color* color, bool isShadow, MaterialPtr* material)
{
	g_font = font;
	g_color = color;
	g_material = material;
	Font_drawCached_real(font, textStr, xOffset, yOffset, color, isShadow, material);
}

void Player$readAdditionalSaveData(Player* player, CompoundTag* tag)
{
	//DO My STUFF
	_Player$readAdditionalSaveData(player, tag);
}

void Player$addAdditionalSaveData(Player* player, CompoundTag* tag)
{
	CompoundTag* barrels = (CompoundTag*)newTag(TAG_COMPOUND, "Barrels");

	for(const auto& it : barrel->containers)
	{
		CompoundTag* barrel = (CompoundTag*)newTag(TAG_COMPOUND, it.first);
		if(barrel == NULL)
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Failed to create tag(%d).", TAG_COMPOUND);

		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Barrel ID: %s", barrel->name.c_str());

		CompoundTag_putString(barrel, "LevelName", it.second->levelName);
		CompoundTag_putInt(barrel, "X", it.second->x);
		CompoundTag_putInt(barrel, "Y", it.second->y);
		CompoundTag_putInt(barrel, "X", it.second->z);

		CompoundTag_putInt(barrel, "ItemID", it.second->itemID);
		CompoundTag_putInt(barrel, "ItemDamage", it.second->itemDamage);
		CompoundTag_putInt(barrel, "ItemCount", it.second->itemsCount);
		CompoundTag_putInt(barrel, "MaxItems", it.second->maxItems);
		CompoundTag_putInt(barrel, "MaxStackSize", it.second->maxStackSize);

		barrels->tags[barrel->name] = barrel;
	}
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Passed the saving of containers");
	tag->tags[barrels->name] = barrels;
	_Player$addAdditionalSaveData(player, tag);
}

static void Tile_initTiles_hook() {
	Tile_initTiles_real();

	barrel = new Barrel(barrelTileId);
	Tile::tiles[barrelTileId] = barrel;
	TileItem* barrelItem = new TileItem(barrelTileId - 256);

}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

	void* handle = dlopen("libminecraftpe.so", RTLD_LAZY);

	void* fontDrawCached = dlsym(handle, "_ZN4Font10drawCachedERKSsffRK5ColorbP11MaterialPtr");
	MSHookFunction(fontDrawCached, (void*) &Font_drawCached_hook, (void**) &Font_drawCached_real);

	void* player$readAdditionalSaveData = dlsym(handle, "_ZN6Player22readAdditionalSaveDataEP11CompoundTag");
	MSHookFunction(player$readAdditionalSaveData, (void*) &Player$readAdditionalSaveData, (void**)&_Player$readAdditionalSaveData);

	void* player$addAdditionalSaveData = dlsym(handle, "_ZN6Player21addAdditionalSaveDataEP11CompoundTag");
	MSHookFunction(player$addAdditionalSaveData, (void*)&Player$addAdditionalSaveData, (void**)&_Player$addAdditionalSaveData);

	Font_draw = (void (*) (Font*, std::string*, float, float, Color*)) dlsym(handle, "_ZN4Font4drawERKSsffRK5Color");

	Player_getCarriedItem = (ItemInstance* (*) (Player*)) dlsym(handle, "_ZN6Player14getCarriedItemEv");

	Tag_Tag = (void (*) (Tag*, std::string const&)) dlsym_weak(handle, "_ZN3TagC2ERKSs");
	Tag_newTag = (Tag* (*) (Tag*, char, std::string const&)) dlsym_weak(handle, "_ZN3Tag6newTagEcRKSs");

	ByteTag_vtable = (void**) dlsym_weak(handle, "_ZTV7ByteTag");
	StringTag_vtable = (void**) dlsym_weak(handle, "_ZTV9StringTag");
	IntTag_vtable = (void**) dlsym_weak(handle, "_ZTV6IntTag");
	CompoundTag_vtable = (void**) dlsym_weak(handle, "_ZTV11CompoundTag");

	CompoundTag_copy = (CompoundTag* (*) (CompoundTag*)) dlsym_weak(handle, "_ZNK11CompoundTag4copyEv");
	CompoundTag_getID = (int (*) (CompoundTag*)) dlsym_weak(handle, "_ZNK11CompoundTag5getIdEv");
	CompoundTag_put = (void (*)(CompoundTag*, std::string const&, Tag*)) dlsym_weak(handle, "_ZN11CompoundTag3putERKSsP3Tag");
	CompoundTag_putInt = (void (*)(CompoundTag*, std::string const&, int)) dlsym_weak(handle, "_ZN11CompoundTag6putIntERKSsi");
	CompoundTag_putByte = (void (*) (CompoundTag*, std::string const&, char)) dlsym_weak(handle, "_ZN11CompoundTag7putByteERKSsc");
	CompoundTag_putString = (void (*)(CompoundTag*, std::string const&, std::string const&)) dlsym_weak(handle, "_ZN11CompoundTag9putStringERKSsS1_");

	ItemInstance_setID = (void (*) (ItemInstance*, int)) dlsym(handle, "_ZN12ItemInstance8_setItemEi");
	ItemInstance_getID = (int (*) (ItemInstance*)) dlsym(handle, "_ZNK12ItemInstance5getIdEv");
	ItemInstance_getMaxStackSize =(int (*) (ItemInstance*)) dlsym(handle, "_ZNK12ItemInstance15getMaxStackSizeEv");
	ItemInstance_isStackable = (bool (*) (ItemInstance*)) dlsym(handle, "_ZNK12ItemInstance11isStackableEv");

	FillingContainer_replaceSlot = (void (*) (FillingContainer*, int, ItemInstance*)) dlsym(handle, "_ZN16FillingContainer11replaceSlotEiP12ItemInstance");
	FillingContainer_addItem = (void (*) (FillingContainer*, ItemInstance*)) dlsym(handle, "_ZN16FillingContainer7addItemEP12ItemInstance");
	FillingContainer_setItem = (void (*) (FillingContainer*, int, ItemInstance*)) dlsym(handle, "_ZN16FillingContainer7setItemEiP12ItemInstance");
	FillingContainer_getItem = (ItemInstance* (*) (FillingContainer*, int)) dlsym(handle, "_ZN16FillingContainer7getItemEi");
	FillingContainer_clearSlot = (int (*) (FillingContainer*, int)) dlsym(handle, "_ZN16FillingContainer9clearSlotEi");
	FillingContainer_getFreeSlot = (int (*) (FillingContainer*)) dlsym(handle, "_ZN16FillingContainer11getFreeSlotEv");
	FillingContainer_getSlotWithItem = (int (*) (FillingContainer*, ItemInstance*)) dlsym(handle, "_ZN16FillingContainer25getSlotWithRemainingSpaceERK12ItemInstance");

	Inventory_getSelected = (int (*) (Inventory*)) dlsym(handle, "_ZN9Inventory11getSelectedEv");

	Level_addEntity = (void (*) (Level*, Entity*)) dlsym(handle, "_ZN5Level9addEntityEP6Entity");

	Entity_Factory = (Entity* (*) (int, TileSource*)) dlsym(handle, "_ZN13EntityFactory12CreateEntityEiR10TileSource");
	ItemEntity_getEntityTypeId = (int (*) (ItemEntity*)) dlsym(handle, "_ZNK10ItemEntity15getEntityTypeIdEv");
	Entity_spawnAtLocation = (void (*) (Entity*, ItemInstance*, float)) dlsym(handle, "_ZN6Entity15spawnAtLocationERK12ItemInstancef");
	Entity_setPos = (void (*) (Entity*, float, float, float)) dlsym(handle, "_ZN6Entity6setPosEfff");
	Entity_setSize = (void (*) (Entity*, float, float)) dlsym(handle, " _ZN6Entity7setSizeEff");

	LevelData_getLevelName = (std::string (*) (LevelData*))  dlsym(handle, "_ZN9LevelData12getLevelNameEv");

	Level_getLevelData = (LevelData* (*) (Level*)) dlsym(handle, "_ZN5Level12getLevelDataEv");
	LevelData_setTagData = (void (*)(LevelData*, CompoundTag*)) dlsym(handle, "_ZN9LevelData10setTagDataEP11CompoundTag");
	LevelData_getTagData = (Tag* (*)(LevelData*, CompoundTag*)) dlsym(handle, "_ZN9LevelData10getTagDataEPK11CompoundTag");

	TileSource_getLevel = (Level* (*) (TileSource*)) dlsym(handle, "_ZNK10TileSource8getLevelEv");

	//ItemEnity Size 332,  Player::drop(ItemInstance const*, bool)
	ItemEntity_ItemEntity = (void (*) (ItemEntity*, TileSource*, float, float, float, ItemInstance*)) dlsym(handle, "_ZN10ItemEntityC2ER10TileSourcefffRK12ItemInstance");

	MSHookFunction((void*) &Tile::initTiles, (void*) &Tile_initTiles_hook, (void**) &Tile_initTiles_real);
	return JNI_VERSION_1_2;
}
