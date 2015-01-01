#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <stdlib.h>
#include <mcpe.h>
#include <Substrate.h>
#include <map>
#include <memory>
#include <sstream>

#include "Barrel.h"
#include "NBT/CompoundTag.h"
#include "NBT/IntTag.h"
#include "NBT/ByteTag.h"
#include "NBT/StringTag.h"
#include "Utils.h"
#include "Recipe.h"
#include "mcpe/MinecraftClient.h"

using namespace std;

const std::string NAME = "Barrel";

class TileEntity;
class GameMode;
class LevelSettings;
class TileTessellator;
class Gui;

void (*FillingContainer_replaceSlot)(FillingContainer*, int, ItemInstance*);
int (*FillingContainer_clearSlot)(FillingContainer*, int);
ItemInstance* (*FillingContainer_getItem)(FillingContainer*, int);
void (*FillingContainer_addItem)(FillingContainer*, ItemInstance*);
void (*FillingContainer_setItem)(FillingContainer*, int, ItemInstance*);
int (*FillingContainer_getFreeSlot)(FillingContainer*);

std::map <std::string, std::string>* bl_I18n_strings;

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
void (*TileSource$getTile)(FullTile&, TileSource*, TilePos const&);

void (*Font_drawShadow)(Font*, std::string const&, float, float, Color const&);

void (*ItemEntity_ItemEntity)(ItemEntity*, TileSource*, float, float, float, ItemInstance*);

static void (*_Font$Font)(Font*, void*, std::string const&, Textures*);

static void (*_Minecraft$selectLevel)(Minecraft*, std::string const&, std::string const&, LevelSettings const&);

static void (*_Gui$render)(Gui*, float, bool, int, int);

static void (*_Screen$setSize)(Screen*, int, int);

static void (*_Player$readAdditionalSaveData)(Player*, CompoundTag*);
static void (*_Player$addAdditionalSaveData)(Player*, CompoundTag*);

static void (*_Init$TileEntities)();
static void (*Tile_initTiles_real)();

bool registered = false;
Font* g_font;
Barrel* g_barrel;
Player* g_player = NULL;
Textures* g_textures = NULL;
Level* g_level = NULL;
TileSource* g_tileSource = NULL;
Minecraft* g_minecraft = NULL;
int screen_width = 0;
int screen_height = 0;

static void Font$Font(Font* font, void* options, std::string const& fontPath, Textures* texture)
{
	g_font = font;
	g_textures = texture;
	_Font$Font(font, options, fontPath, texture);
}

static void Tile_initTiles_hook() {
	Tile_initTiles_real();

	g_barrel = new Barrel(BARREL_ID);
	Tile::tiles[BARREL_ID] = g_barrel;

	TileItem* barrelItem = new TileItem(BARREL_ID - 256);

}

static void Gui$render(Gui* gui, float wtf, bool idk, int idk2, int idk3)
{
	_Gui$render(gui, wtf, idk, idk2, idk3);
	if(g_tileSource != NULL)
	{
		HitResult* result = (HitResult*) ((uintptr_t) g_level + MINECRAFT_HIT_RESULT_OFFSET);
		FullTile tile;
		TileSource$getTile(tile, g_tileSource, result->tile);
		if(result->type == HitResultType::TILE &&  tile.id == BARREL_ID)
		{
			std::string id = getIdentifier(g_level, result->tile.x, result->tile.y, result->tile.z);
			Container* container = g_barrel->containers.at(id);
			if(container != NULL && container->itemID > 0)
			{
				Color color;
				if(container->itemID == container->maxItems)
				{
					color.r = 255;
					color.g = 0;
					color.b = 0;
					color.a = 255;
				}
				else if((container->itemsCount / container->maxItems) >= 0.80)
				{
					color.r = 255;
					color.g = 255;
					color.b = 0;
					color.a = 255;
				}
				else
				{
					color.r = 0;
					color.g = 255;
					color.b = 0;
					color.a = 255;
				}

				std::stringstream str;
				str << "Items Count: " << container->itemsCount << "/" << container->maxItems;
				str << "(" << ((float)(container->itemsCount / container->maxStackSize)) << "/" << (container->maxItems / container->maxStackSize) << ")";
				std::string temp = str.str();
				Font_drawShadow(g_font, temp, ((screen_width / 2) - (temp.length() * 2)), 10, color);
				ItemRenderer::singleton().renderGuiItemNew(g_textures, new ItemInstance(container->itemID, 1, container->itemDamage), 0, (screen_width / 2), 30, 1, 2, 2);
			}
		}
	}
}

static void Minecraft$selectLevel(Minecraft* minecraft, std::string const& wtf1, std::string const& wtf2, LevelSettings const& settings)
{
	g_minecraft = minecraft;
	if(!registered)
	{
		registered = true;

		//g_barrel->setDescriptionId(NAME);
		(*bl_I18n_strings)["tile." + NAME + ".name"] = NAME;

		std::vector<std::string> shape = { "wsw", "w w", "www"};
		std::vector<Recipes::Type> ingredients;
		Recipes::Type wood;
		wood.c = 'w';
		wood.item = NULL;
		wood.tile = Tile::tiles[17];
		ingredients.push_back(wood);

		Recipes::Type slab;
		slab.c = 's';
		slab.item = Item::items[158];
		slab.tile = NULL;
		ingredients.push_back(slab);

		Recipes::getInstance()->addShapedRecipe((*create_ItemInstance(BARREL_ID, 1, 0)), shape, ingredients);
	}
	_Minecraft$selectLevel(minecraft, wtf1, wtf2, settings);
}

static void Screen$setSize(Screen* screen, int width, int height)
{
	_Screen$setSize(screen, width, height);
	screen_width = width;
	screen_height = height;
}


static void Init$TileEntities()
{
	_Init$TileEntities();
	//TODO: Register my tile entities
}

void Player$readAdditionalSaveData(Player* player, CompoundTag* tag)
{
	g_player = player;
	g_level = getLevel(player);
	g_tileSource = getTileSource(g_level);


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

	bl_I18n_strings = (std::map <std::string, std::string> *) dlsym(RTLD_DEFAULT, "_ZN4I18n8_stringsE");

	Font_drawShadow = (void (*) (Font*, std::string const&, float, float, Color const&)) dlsym(RTLD_DEFAULT, "_ZN4Font4drawERKSsffRK5Color");

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
	TileSource$getTile = (void (*) (FullTile&, TileSource*, TilePos const&)) dlsym(RTLD_DEFAULT, "_ZN10TileSource7getTileERK7TilePos");

	Entity_setPos = (void (*) (Entity*, float, float, float)) dlsym(RTLD_DEFAULT, "_ZN6Entity6setPosEfff");

	LevelData_getLevelName = (std::string (*) (LevelData*))  dlsym(RTLD_DEFAULT, "_ZN9LevelData12getLevelNameEv");

	Level_getLevelData = (LevelData* (*) (Level*)) dlsym(RTLD_DEFAULT, "_ZN5Level12getLevelDataEv");
	LevelData_setTagData = (void (*)(LevelData*, CompoundTag*)) dlsym(RTLD_DEFAULT, "_ZN9LevelData10setTagDataEP11CompoundTag");
	LevelData_getTagData = (Tag* (*)(LevelData*, CompoundTag*)) dlsym(RTLD_DEFAULT, "_ZN9LevelData10getTagDataEPK11CompoundTag");

	TileSource_getLevel = (Level* (*) (TileSource*)) dlsym(RTLD_DEFAULT, "_ZNK10TileSource8getLevelEv");

	//ItemEnity Size 332,  Player::drop(ItemInstance const*, bool)
	ItemEntity_ItemEntity = (void (*) (ItemEntity*, TileSource*, float, float, float, ItemInstance*)) dlsym(RTLD_DEFAULT, "_ZN10ItemEntityC2ER10TileSourcefffRK12ItemInstance");

	void* font$Font = dlsym(RTLD_DEFAULT, "_ZN4FontC2EP7OptionsRKSsP8Textures");
	MSHookFunction(font$Font, (void*) &Font$Font, (void**)&_Font$Font);

	void* minecraft$selectLevel = dlsym(RTLD_DEFAULT, "_ZN9Minecraft11selectLevelERKSsS1_RK13LevelSettings");
	MSHookFunction(minecraft$selectLevel, (void*)&Minecraft$selectLevel, (void**)&_Minecraft$selectLevel);

	void* screen$setSize = dlsym(RTLD_DEFAULT, "_ZN6Screen7setSizeEii");
	MSHookFunction(screen$setSize, (void*)&Screen$setSize, (void**)&_Screen$setSize);

	void* gui$render = dlsym(RTLD_DEFAULT, "_ZN3Gui6renderEfbii");
	MSHookFunction(gui$render, (void*)&Gui$render, (void**)&_Gui$render);

	void* init$TileEntities = dlsym(RTLD_DEFAULT, "_ZN10TileEntity16initTileEntitiesEv");
	MSHookFunction(init$TileEntities, (void*)&Init$TileEntities, (void**)&_Init$TileEntities);

	void* player$readAdditionalSaveData = dlsym(RTLD_DEFAULT, "_ZN6Player22readAdditionalSaveDataEP11CompoundTag");
	MSHookFunction(player$readAdditionalSaveData, (void*) &Player$readAdditionalSaveData, (void**)&_Player$readAdditionalSaveData);

	void* player$addData = dlsym(RTLD_DEFAULT, "_ZN6Player21addAdditionalSaveDataEP11CompoundTag");
	MSHookFunction(player$addData, (void*)&Player$addAdditionalSaveData, (void**)&_Player$addAdditionalSaveData);

	MSHookFunction((void*) &Tile::initTiles, (void*) &Tile_initTiles_hook, (void**) &Tile_initTiles_real);
	return JNI_VERSION_1_2;
}
