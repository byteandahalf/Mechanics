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

#include "Mechanics.h"
#include "MCPE/client/MinecraftClient.h"
#include "MCPE/world/level/block/Block.h"
#include "MCPE/world/item/Item.h"
#include "MCPE/world/level/biome/BiomeDecorator.h"



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
}

//This thing crash I don't know why!!
void (*_Item$addBlockItems)();
void Item$addBlockItems()
{
	_Item$addBlockItems();

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

//Item::addBlockItems - Crash I don't know why...


#if defined(ABI_X86)

extern "C" 
{
	void mod_init() 
	{
		mcpelauncher_hook((void*) &Item::initItems, (void*) &Item$initItems, (void**) &_Item$initItems);
		mcpelauncher_hook((void*) &Block::initBlocks, (void*) &Block$initBlocks, (void**) &_Block$initBlocks);
		//mcpelauncher_hook((void*) &Item::addBlockItems, (void*) &Item$addBlockItems, (void**) &_Item$addBlockItems);
		mcpelauncher_hook((void*) &Item::initCreativeItems, (void*) &Item$initCreativeItems, (void**) &_Item$initCreativeItems);
		//mcpelauncher_hook((void*) &Recipies::initRecipies, (void*) &Recipies$initRecipies, (void**) &_Recipies$initRecipies);
	}
}

#elif defined(ABI_ARM)

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	
	MSHookFunction((void*) &Item::initItems, (void*) &Item$initItems, (void**) &_Item$initItems);
	MSHookFunction((void*) &Block::initBlocks, (void*) &Block$initBlocks, (void**) &_Block$initBlocks);
	//MSHookFunction((void*) &Item::addBlockItems, (void*) &Item$addBlockItems, (void**) &_Item$addBlockItems);
	MSHookFunction((void*) &Item::initCreativeItems, (void*) &Item$initCreativeItems, (void**) &_Item$initCreativeItems);
	//MSHookFunction((void*) &Recipies::initRecipies, (void*) &Recipies$initRecipies, (void**) &_Recipies$initRecipies);

	return JNI_VERSION_1_2;
}

#endif