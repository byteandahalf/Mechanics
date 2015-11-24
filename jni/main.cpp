#include <stdlib.h>
#include <string>
#include <memory>

#if defined(ABI_X86)
	#include "mcpelauncher_api.h"
#elif defined(ABI_ARM)
	#include <jni.h>
	#include <substrate.h>
#endif

#include "Common.h"

#include "MCPE/client/MinecraftClient.h"
#include "MCPE/locale/I18n.h"
#include "MCPE/world/item/Item.h"
#include "MCPE/world/level/block/Block.h"

#include "Mechanics.h"
#include "Mechanics/blocks/entity/BarrelEntity.h"
#include "Mechanics/blocks/entity/GrinderEntity.h"

Mechanics* mechanics;

void (*_MinecraftClient$init)(MinecraftClient*);
void MinecraftClient$init(MinecraftClient* minecraftClient)
{
	mechanics = new Mechanics();

	_MinecraftClient$init(minecraftClient);

	mechanics->initRecipies();
}

void (*_Item$initItems)();
void Item$initItems()
{
	_Item$initItems();

	mechanics->initItems();
}

void (*_Block$initBlocks)();
void Block$initBlocks() 
{
	_Block$initBlocks();

	mechanics->initBlocks();
	mechanics->initBlockItems();
}

void (*_Item$initCreativeItems)();
void Item$initCreativeItems() 
{
	_Item$initCreativeItems();

	mechanics->initCreativeItems();
}

void (*_Recipies$initRecipies)();
void Recipies$initRecipies()
{
	_Recipies$initRecipies();

	mechanics->initRecipies();
}

void (*_BlockEntity$initBlockEntities)();
void BlockEntity$initBlockEntities()
{
	_BlockEntity$initBlockEntities();

	mechanics->initBlockEntities();
}

std::unique_ptr<BlockEntity> (*_BlockEntityFactory$createBlockEntity)(BlockEntityType, const BlockPos&);
std::unique_ptr<BlockEntity> BlockEntityFactory$createBlockEntity(BlockEntityType type, const BlockPos&  pos)
{
	if(type == BlockEntityType::Barrel) return std::unique_ptr<BlockEntity>(new BarrelEntity(pos));
	if(type == BlockEntityType::Grinder) return std::unique_ptr<BlockEntity>(new GrinderEntity(pos));
	return _BlockEntityFactory$createBlockEntity(type, pos);
}

std::string (*_I18n$get)(std::string const&, std::vector<std::string> const&);
std::string I18n$get(std::string const& key, std::vector<std::string> const& a) 
{
	if(key == "tile.grinder.name") return "Grinder";
	if(key == "tile.barrel.name") return "Barrel";
	if(key == "tile.wooden_crank.name") return "Wooden Crank";
	if(key == "item.ironDust.name") return "Iron Dust";
	if(key == "item.goldDust.name") return "Gold Dust";
	return _I18n$get(key, a);
}


//TODO: Fix hooking bug in MCPELauncher-Linux
#if defined(ABI_X86)

extern "C" 
{
	void mod_init() 
	{
		mcpelauncher_hook((void*) &MinecraftClient::init, (void*) &MinecraftClient$init, (void**) &_MinecraftClient$init);
		mcpelauncher_hook((void*) &Block::initBlocks, (void*) &Block$initBlocks, (void**) &_Block$initBlocks);
		//mcpelauncher_hook((void*) &BlockEntity::initBlockEntities, (void*) &BlockEntity$initBlockEntities, (void**) &_BlockEntity$initBlockEntities);
		mcpelauncher_hook((void*) &Item::initItems, (void*) &Item$initItems, (void**) &_Item$initItems);
		mcpelauncher_hook((void*) &Item::initCreativeItems, (void*) &Item$initCreativeItems, (void**) &_Item$initCreativeItems);
		//mcpelauncher_hook((void*) &Recipies::initRecipies, (void*) &Recipies$initRecipies, (void**) &_Recipies$initRecipies);
		
		//mcpelauncher_hook((void*) &I18n::get, (void*) &I18n$get, (void**) &_I18n$get);

		//mcpelauncher_hook((void*) &BlockEntityFactory::createBlockEntity, (void*) &BlockEntityFactory$createBlockEntity, (void**) &_BlockEntityFactory$createBlockEntity);
	}
}

#elif defined(ABI_ARM)

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) 
{
	MSHookFunction((void*) &MinecraftClient::init, (void*) &MinecraftClient$init, (void**) &_MinecraftClient$init);
	MSHookFunction((void*) &Block::initBlocks, (void*) &Block$initBlocks, (void**) &_Block$initBlocks);
	MSHookFunction((void*) &BlockEntity::initBlockEntities, (void*) &BlockEntity$initBlockEntities, (void**) &_BlockEntity$initBlockEntities);
	MSHookFunction((void*) &Item::initItems, (void*) &Item$initItems, (void**) &_Item$initItems);
	MSHookFunction((void*) &Item::initCreativeItems, (void*) &Item$initCreativeItems, (void**) &_Item$initCreativeItems);
	//MSHookFunction((void*) &Recipies::initRecipies, (void*) &Recipies$initRecipies, (void**) &_Recipies$initRecipies);
	
	MSHookFunction((void*) &I18n::get, (void*) &I18n$get, (void**) &_I18n$get);

	MSHookFunction((void*) &BlockEntityFactory::createBlockEntity, (void*) &BlockEntityFactory$createBlockEntity, (void**) &_BlockEntityFactory$createBlockEntity);


	return JNI_VERSION_1_2;
}

#endif