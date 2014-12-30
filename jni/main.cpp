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
#include "Recipe.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))

const std::string NAME = "Barrel";

class TileEntity;
class GameMode;

void (*FillingContainer_replaceSlot)(FillingContainer*, int, ItemInstance*);
int (*FillingContainer_clearSlot)(FillingContainer*, int);
ItemInstance* (*FillingContainer_getItem)(FillingContainer*, int);
void (*FillingContainer_addItem)(FillingContainer*, ItemInstance*);
void (*FillingContainer_setItem)(FillingContainer*, int, ItemInstance*);
int (*FillingContainer_getFreeSlot)(FillingContainer*);

std::map <std::string, std::string>* bl_I18n_strings;
static Recipes* (*bl_Recipes_getInstance)();
static void (*bl_Recipes_addShapedRecipe)(Recipes*, std::vector<ItemInstance> const&, std::vector<std::string> const&,
	std::vector<Recipes::Type> const&);

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

static void (*_GameMode$GameMode)(GameMode* gamemode, Minecraft* minecraft);

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

	g_barrel->setDescriptionId(NAME);
	(*bl_I18n_strings)["tile." + NAME + ".name"] = NAME;

	TileItem* barrelItem = new TileItem(barrelTileId - 256);

}

static void GameMode$GameMode(GameMode* gamemode, Minecraft* minecraft)
{
	std::vector<ItemInstance> output = { (*create_ItemInstance(barrelTileId, 1, 0)) };

	std::vector<std::string> shape;
	shape.push_back("wsw");
	shape.push_back("w w");
	shape.push_back("www");

	std::vector<Recipes::Type> ingredients;
	Recipes::Type wood;
	wood.c = 'w';
	wood.item = NULL;
	wood.tile = NULL;
	wood.itemInstance = (*create_ItemInstance(17, 1, 0));
	ingredients.push_back(wood);

	Recipes::Type slab;
	slab.c = 's';
	slab.item = NULL;
	slab.tile = NULL;
	slab.itemInstance = (*create_ItemInstance(158, 1, 0));
	ingredients.push_back(slab);

	Recipes* recipes = bl_Recipes_getInstance();
	if(recipes != NULL)
		bl_Recipes_addShapedRecipe(recipes, output, shape, ingredients);
	else
		LOGI("NULL");

	_GameMode$GameMode(gamemode, minecraft);

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
		if(it.second->itemID == 0)
			continue;

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

	void* gameMode$gameMode = dlsym(RTLD_DEFAULT, "_ZN8GameModeC2EP9Minecraft");
	MSHookFunction(gameMode$gameMode, (void*) &GameMode$GameMode, (void**)&_GameMode$GameMode);

	void* init$TileEntities = dlsym(RTLD_DEFAULT, "_ZN10TileEntity16initTileEntitiesEv");
	MSHookFunction(init$TileEntities, (void*)&Init$TileEntities, (void**)&_Init$TileEntities);

	void* player$readAdditionalSaveData = dlsym(RTLD_DEFAULT, "_ZN6Player22readAdditionalSaveDataEP11CompoundTag");
	MSHookFunction(player$readAdditionalSaveData, (void*) &Player$readAdditionalSaveData, (void**)&_Player$readAdditionalSaveData);

	void* player$addData = dlsym(RTLD_DEFAULT, "_ZN6Player21addAdditionalSaveDataEP11CompoundTag");
	MSHookFunction(player$addData, (void*)&Player$addAdditionalSaveData, (void**)&_Player$addAdditionalSaveData);

	bl_I18n_strings = (std::map <std::string, std::string> *) dlsym(RTLD_DEFAULT, "_ZN4I18n8_stringsE");

	bl_Recipes_getInstance = (Recipes* (*)()) dlsym(RTLD_DEFAULT, "_ZN7Recipes11getInstanceEv");
	bl_Recipes_addShapedRecipe = (void (*)(Recipes*, std::vector<ItemInstance> const&, std::vector<std::string> const&,
			std::vector<Recipes::Type> const&)) dlsym(RTLD_DEFAULT,
			"_ZN7Recipes15addShapedRecipeERKSt6vectorI12ItemInstanceSaIS1_EERKS0_ISsSaISsEERKS0_INS_4TypeESaISA_EE");

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
