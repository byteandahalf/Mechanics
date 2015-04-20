#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <sstream>

#include "Substrate.h"
#include "mcpe/Recipes.h"
#include "tile/Barrel.h"
#include "Utils.h"
#include "tile/entity/BarrelEntity.h"
#include "mcpe/item/TileItem.h"
#include "mcpe/tile/entity/SignTileEntity.h"
#include "mcpe/tile/entity/ChestTileEntity.h"
#include "mcpe/tile/entity/FurnaceTileEntity.h"
#include "mcpe/tile/entity/MobSpawnerTileEntity.h"
#include "mcpe/tile/entity/NetherReactorTileEntity.h"

#include "mcpe/gui/gui/Gui.h"
#include "mcpe/item/ItemRenderer.h"
#include "mcpe/game/MinecraftClient.h"
#include "mcpe/entity/item/ItemEntity.h"

#define MINECRAFT_HIT_RESULT_OFFSET 2680

int screen_width = 0;
int screen_height = 0;
bool registered;

static void (*_Gui$render)(Gui*, float, bool, int, int);
static void Gui$render(Gui* gui, float wtf, bool idk, int idk2, int idk3)
{
	_Gui$render(gui, wtf,idk, idk2, idk3);
	
	Player* player = gui->minecraft.getPlayer();
	TileSource* tileSource = player->region;
	HitResult* result = (HitResult*) ((uintptr_t) player->level + MINECRAFT_HIT_RESULT_OFFSET);// I don't know why `&player->level->hitResult` don't work. Maybe outdated class?

	Tile* tile = tileSource->getTilePtr(result->tile);
	if(result->type == HitResultType::TILE && tile ==  Tile::tiles[Barrel::BARREL_ID])
	{
		BarrelEntity* container = (BarrelEntity*)tileSource->getTileEntity(result->tile);
		if(container != NULL && container->itemInstance->getId() != 0 )
		{
			Color color;
			if(container->itemCount == container->maxItems)
			{
				color.r = 255;
				color.g = 0;
				color.b = 0;
				color.a = 255;
			}
			else if((container->itemCount / container->maxItems) >= 0.80)
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
			str << "Items Count: " << container->itemCount << "/" << container->maxItems;
			str << "(" << ((float)(container->itemCount / container->itemInstance->getMaxStackSize())) << "/" << (container->maxItems / container->itemInstance->getMaxStackSize()) << ")";
			std::string temp = str.str();
			gui->minecraft.font->draw(temp, ((screen_width / 2) - (temp.length() * 2)), 10, color);
			ItemRenderer::singleton().renderGuiItemNew(gui->minecraft.textures.get(), new ItemInstance(container->itemInstance->getId(), 1, container->itemInstance->auxValue), 0, (screen_width / 2), 30, 1, 2, 2);
		}
	}

}


static void (*_Screen$setSize)(Screen*, int, int);
static void Screen$setSize(Screen* screen, int width, int height)
{
	_Screen$setSize(screen, width, height);
	screen_width = width;
	screen_height = height;
}

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

static void (*_SurvivalScreen_updateCraftingItem)(void*, bool);
static void SurvivalScreen_updateCraftingItem(void* screen, bool idk)
{
	_SurvivalScreen_updateCraftingItem(screen, idk);

	if(registered)
		return;
	
	
	std::vector<std::string> shape  = { "wsw", "w w", "www"};
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
	
	Recipes::getInstance()->addShapedRecipe(ItemInstance(Barrel::BARREL_ID, 1, 0), shape, ingredients);
	registered = true;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    MSHookFunction((void*)&Item::initItems, (void*)&Item$initItems, (void**)&_Item$initItems);
    MSHookFunction((void*)&Tile::initTiles, (void*)&Tile$initTiles, (void**)&_Tile$initTiles);
    MSHookFunction((void*)&TileEntity::initTileEntities, (void*)&TileEntity$initTileEntities, (void**)&_TileEntity$initEntities);

    void* tileEntityFactory$createTileEntity = dlsym(RTLD_DEFAULT, "_ZN17TileEntityFactory16createTileEntityE14TileEntityTypeRK7TilePos");
    MSHookFunction(tileEntityFactory$createTileEntity, (void*)&TileEntityFactory$createTileEntity, (void**)&_TileEntityFactory$createTileEntity);

    void* I18n$get_ = dlsym(RTLD_DEFAULT, "_ZN4I18n3getERKSs");
    MSHookFunction(I18n$get_, (void*) &I18n$get, (void**) &_I18n$get);

    void* screen$setSize = dlsym(RTLD_DEFAULT, "_ZN6Screen7setSizeEii");
	MSHookFunction(screen$setSize, (void*)&Screen$setSize, (void**)&_Screen$setSize);

	void* gui$render = dlsym(RTLD_DEFAULT, "_ZN3Gui6renderEfbii");
	MSHookFunction(gui$render, (void*)&Gui$render, (void**)&_Gui$render);
	
	void* survival_item = dlsym(RTLD_DEFAULT, "_ZN23SurvivalInventoryScreen20updateCraftableItemsEb");
	MSHookFunction(survival_item, (void*)&SurvivalScreen_updateCraftingItem, (void**)&_SurvivalScreen_updateCraftingItem);

	return JNI_VERSION_1_2;
}
