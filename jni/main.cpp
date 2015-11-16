#include <stdlib.h>
#include <string>
#include <memory>

#if defined(ABI_X86)
	#include "mcpelauncher_api.h"
#elif defined(ABI_ARM)
	#include <android/log.h>
	#include <dlfcn.h>
	#include <jni.h>
	#include <substrate.h>
#endif

#include "MCPE/client/MinecraftClient.h"
#include "MCPE/world/item/Item.h"
#include "MCPE/world/level/block/Block.h"
#include "MCPE/world/level/block/entity/BlockEntity.h"

#include "Mechanics.h"
#include "Mechanics/blocks/entity/BarrelEntity.h"


void (*_Item$initItems)();
void Item$initItems()
{
	_Item$initItems();

	Mechanics::initItems();
}

void (*_Block$initBlocks)();
void Block$initBlocks() 
{
	_Block$initBlocks();

	Mechanics::initBlocks();
	Mechanics::addBlockItems();
}

void (*_Item$initCreativeItems)();
void Item$initCreativeItems() 
{
	_Item$initCreativeItems();

	Mechanics::initCreativeItems();
}

void (*_Recipies$initRecipies)();
void Recipies$initRecipies()
{
	_Recipies$initRecipies();

	Mechanics::initRecipies();
}

static void (*_BlockEntity$initBlockEntities)();
static void BlockEntity$initBlockEntities()
{
	_BlockEntity$initBlockEntities();

	Mechanics::initBlockEntities();
}

static std::unique_ptr<BlockEntity> (*_BlockEntityFactory$createBlockEntity)(BlockEntityType, BlockPos const&);
static std::unique_ptr<BlockEntity>  BlockEntityFactory$createBlockEntity(BlockEntityType type, BlockPos const& pos)
{
	if(type == BlockEntityType::Barrel) return std::unique_ptr<BlockEntity>(new BarrelEntity(pos));
	return _BlockEntityFactory$createBlockEntity(type, pos);
}


#if defined(ABI_X86)

extern "C" 
{
	void mod_init() 
	{
		mcpelauncher_hook((void*) &Item::initItems, (void*) &Item$initItems, (void**) &_Item$initItems);
		mcpelauncher_hook((void*) &Block::initBlocks, (void*) &Block$initBlocks, (void**) &_Block$initBlocks);
		mcpelauncher_hook((void*) &Item::initCreativeItems, (void*) &Item$initCreativeItems, (void**) &_Item$initCreativeItems);
		//mcpelauncher_hook((void*) &Recipies::initRecipies, (void*) &Recipies$initRecipies, (void**) &_Recipies$initRecipies);
		
		//mcpelauncher_hook((void*) &BlockEntity::initBlockEntities, (void*) &BlockEntity$initBlockEntities, (void**) &_BlockEntity$initBlockEntities);
		//mcpelauncher_hook((void*) &BlockEntityFactory::createBlockEntity, (void*) &BlockEntityFactory$createBlockEntity, (void**) &_BlockEntityFactory$createBlockEntity);
	}
}

#elif defined(ABI_ARM)

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	
	MSHookFunction((void*) &Item::initItems, (void*) &Item$initItems, (void**) &_Item$initItems);
	MSHookFunction((void*) &Block::initBlocks, (void*) &Block$initBlocks, (void**) &_Block$initBlocks);
	MSHookFunction((void*) &Item::initCreativeItems, (void*) &Item$initCreativeItems, (void**) &_Item$initCreativeItems);
	//MSHookFunction((void*) &Recipies::initRecipies, (void*) &Recipies$initRecipies, (void**) &_Recipies$initRecipies);


	return JNI_VERSION_1_2;
}

#endif