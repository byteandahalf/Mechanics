#pragma once

#include "MCPE/world/level/block/Block.h"
#include "MCPE/world/item/Item.h"

class Mechanics
{
public:

	/////////////////////
	//Blocks          ///
	/////////////////////
	static Block* mBarrel; 
	static Block* mGrinder;
	static Block* mPowerCell;
	static Block* mConductiveCable;
	static Block* mCrusher;
	static Block* mElectricFurnace;


	/////////////////////
	//Items           ///
	/////////////////////
	static Item* mOreDust;
	static Item* mWrench;
	static Item* mCable;


	//This method will instatiate all block
	static void initBlocks();

	//This method will instatiate all item
	static void initItems();

	//This method will add the BlockItem to the array
	static void addBlockItems();

	//This method will add all item and block from Mehchanics o the creative inventory
	static void initCreativeItems();

	//This method will register all item and block recipes
	static void initRecipies();

	//Register all the BlockEntities
	static void initBlockEntities();


};