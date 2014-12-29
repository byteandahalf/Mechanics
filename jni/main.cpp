#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <stdlib.h>
#include <mcpe.h>
#include <Substrate.h>
#include <map>

#include "Barrel.h"
#include "NBT/CompoundTag.h"
#include "NBT/IntTag.h"
#include "NBT/ByteTag.h"
#include "NBT/StringTag.h"
#include "Utils.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))

class TileEntity;

void (*FillingContainer_replaceSlot)(FillingContainer*, int, ItemInstance*);
int (*FillingContainer_clearSlot)(FillingContainer*, int);
ItemInstance* (*FillingContainer_getItem)(FillingContainer*, int);
void (*FillingContainer_addItem)(FillingContainer*, ItemInstance*);
void (*FillingContainer_setItem)(FillingContainer*, int, ItemInstance*);
int (*FillingContainer_getFreeSlot)(FillingContainer*);

ItemInstance* (*Player_getCarriedItem)(Player*);

void (*ItemInstance_setID)(ItemInstance*, int);
int (*ItemInstance_getID)(ItemInstance*);
int (*ItemInstance_getMaxStackSize)(ItemInstance*);
bool (*ItemInstance_isStackable)(ItemInstance*);

void (*Level_addEntity)(Level*, Entity*);

void (*Entity_setPos)(Entity*, float, float, float);

LevelData* (*Level_getLevelData)(Level*);

std::string (*LevelData_getLevelName)(LevelData*);
void (*LevelData_setTagData)(LevelData*, CompoundTag*);
Tag* (*LevelData_getTagData)(LevelData*, CompoundTag*);

Level* (*TileSource_getLevel)(TileSource*);

void (*Font_draw)(Font*, std::string const&, float, float, Color*);

void (*ItemEntity_ItemEntity)(ItemEntity*, TileSource*, float, float, float, ItemInstance*);

static void (*_Font$Font)(Font*, void*, std::string const&, void*);
static TileEntity* (*_TileEntityFactory)(short, TilePos const&);

static void (*_Player$readAdditionalSaveData)(Player*, CompoundTag*);
static void (*_Player$addAdditionalSaveData)(Player*, CompoundTag*);

static void (*_Init$TileEntities)();
static void (*Tile_initTiles_real)();

const int barrelTileId = 201;
Font* g_font;
Barrel* g_barrel;

static void Font$Font(Font* font, void* options, std::string const& fontPath, void* texture)
{
	g_font = font;
	_Font$Font(font, options, fontPath, texture);
}

static void Tile_initTiles_hook() {
	Tile_initTiles_real();

	g_barrel = new Barrel(barrelTileId);
	Tile::tiles[barrelTileId] = g_barrel;
	TileItem* barrelItem = new TileItem(barrelTileId - 256);

}

static void Init$TileEntities()
{
	_Init$TileEntities();
	//TODO: Register my tile entities
}

// static TileEntity* TileEntityFactory(short tileEntityType, TilePos const& pos)
// {
// 	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "TileEntity: %d", tileEntityType);
// 	//TODO: if tileEntityType = 5, then return new BarrelTileEntity
// 	return _TileEntityFactory(tileEntityType, pos);
// }

void Player$readAdditionalSaveData(Player* player, CompoundTag* tag)
{
	CompoundTag* mainTag = (CompoundTag*)tag->tags["Barrels"];
	if(mainTag != NULL)
	{
		for(const auto& it : mainTag->tags)
		{
			CompoundTag* barrel = (CompoundTag*)it.second;
			std::string levelName = ((StringTag*)barrel->tags["LevelName"])->data;

			IntTag* x = (IntTag*)barrel->tags["X"];
			IntTag* y = (IntTag*)barrel->tags["Y"];
			IntTag* z = (IntTag*)barrel->tags["Z"];

			IntTag* itemID = (IntTag*)barrel->tags["ItemID"];
			IntTag* itemDamage = (IntTag*)barrel->tags["ItemDamage"];
			IntTag* itemCount = (IntTag*)barrel->tags["ItemCount"];

			IntTag* maxItems = (IntTag*)barrel->tags["MaxItems"];
			IntTag* maxStackSize = (IntTag*)barrel->tags["MaxStackSize"];
			ByteTag* locked = (ByteTag*)barrel->tags["Locked"];

			Container* cont = new Container(levelName, x->data, y->data, z->data);
			cont->itemID = itemID->data;
			cont->itemDamage = itemDamage->data;
			cont->itemsCount = itemCount->data;

			cont->maxItems = maxItems->data;
			cont->maxStackSize = maxStackSize->data;
			cont->locked = locked->data == 1 ? true : false;

			g_barrel->containers[it.first] = cont;

		}
	}
	_Player$readAdditionalSaveData(player, tag);
}


static void Player$addAdditionalSaveData(Player* player, CompoundTag* tag)
{
	CompoundTag* mainTag = (CompoundTag*)Tag::newTag(TAG_COMPOUND, "Barrels");
	for(const auto& it : g_barrel->containers)
	{
		CompoundTag* barrel = (CompoundTag*)Tag::newTag(TAG_COMPOUND, it.first);
		barrel->putString("LevelName", it.second->levelName);
		barrel->putInt("X", it.second->x);
		barrel->putInt("Y", it.second->y);
		barrel->putInt("Z", it.second->z);

		barrel->putInt("ItemID", it.second->itemID);
		barrel->putInt("ItemDamage", it.second->itemDamage);
		barrel->putInt("ItemCount", it.second->itemsCount);
		barrel->putInt("MaxItems", it.second->maxItems);
		barrel->putInt("MaxStackSize", it.second->maxStackSize);

		barrel->putByte("Locked", (char)it.second->locked);

		mainTag->put(it.first, barrel);
	}

	tag->put("Barrels", mainTag);
	_Player$addAdditionalSaveData(player, tag);
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

	void* font$Font = dlsym(RTLD_DEFAULT, "_ZN4FontC2EP7OptionsRKSsP8Textures");
	MSHookFunction(font$Font, (void*) &Font$Font, (void**)&_Font$Font);

	// void* tileEntityFactory$createTileEntity = dlsym(RTLD_DEFAULT, "_ZN17TileEntityFactory16createTileEntityE14TileEntityTypeRK7TilePos");
	// MSHookFunction(tileEntityFactory$createTileEntity, (void*) &TileEntityFactory, (void**) &_TileEntityFactory);

	void* init$TileEntities = dlsym(RTLD_DEFAULT, "_ZN10TileEntity16initTileEntitiesEv");
	MSHookFunction(init$TileEntities, (void*)&Init$TileEntities, (void**)&_Init$TileEntities);

	void* player$readAdditionalSaveData = dlsym(RTLD_DEFAULT, "_ZN6Player22readAdditionalSaveDataEP11CompoundTag");
	MSHookFunction(player$readAdditionalSaveData, (void*) &Player$readAdditionalSaveData, (void**)&_Player$readAdditionalSaveData);

	void* player$addData = dlsym(RTLD_DEFAULT, "_ZN6Player21addAdditionalSaveDataEP11CompoundTag");
	MSHookFunction(player$addData, (void*)&Player$addAdditionalSaveData, (void**)&_Player$addAdditionalSaveData);

	Font_draw = (void (*) (Font*, std::string const&, float, float, Color*)) dlsym(RTLD_DEFAULT, "_ZN4Font4drawERKSsffRK5Color");

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

	Level_addEntity = (void (*) (Level*, Entity*)) dlsym(RTLD_DEFAULT, "_ZN5Level9addEntityEP6Entity");

	Entity_setPos = (void (*) (Entity*, float, float, float)) dlsym(RTLD_DEFAULT, "_ZN6Entity6setPosEfff");

	LevelData_getLevelName = (std::string (*) (LevelData*))  dlsym(RTLD_DEFAULT, "_ZN9LevelData12getLevelNameEv");

	Level_getLevelData = (LevelData* (*) (Level*)) dlsym(RTLD_DEFAULT, "_ZN5Level12getLevelDataEv");
	LevelData_setTagData = (void (*)(LevelData*, CompoundTag*)) dlsym(RTLD_DEFAULT, "_ZN9LevelData10setTagDataEP11CompoundTag");
	LevelData_getTagData = (Tag* (*)(LevelData*, CompoundTag*)) dlsym(RTLD_DEFAULT, "_ZN9LevelData10getTagDataEPK11CompoundTag");

	TileSource_getLevel = (Level* (*) (TileSource*)) dlsym(RTLD_DEFAULT, "_ZNK10TileSource8getLevelEv");

	//ItemEnity Size 332,  Player::drop(ItemInstance const*, bool)
	ItemEntity_ItemEntity = (void (*) (ItemEntity*, TileSource*, float, float, float, ItemInstance*)) dlsym(RTLD_DEFAULT, "_ZN10ItemEntityC2ER10TileSourcefffRK12ItemInstance");

	MSHookFunction((void*) &Tile::initTiles, (void*) &Tile_initTiles_hook, (void**) &Tile_initTiles_real);
	return JNI_VERSION_1_2;
}
