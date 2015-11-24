#pragma once

#include "MCPE/world/level/block/Block.h"
#include "MCPE/world/item/Item.h"
#include "Mechanics/recipe/RecipeManager.h"

class Mechanics
{
private:
	RecipeManager* _recipesManager;

public:
	Mechanics();

	//This method will instatiate all block
	void initBlocks();

	//This method will instatiate all item
	void initItems();

	//This method will add the BlockItem to the array
	void initBlockItems();

	//This method will add all item and block from Mehchanics o the creative inventory
	void initCreativeItems();

	//This method will register all item and block recipes
	void initRecipies();

	//Register all the BlockEntities
	void initBlockEntities();

	RecipeManager* getRecipeManager() const;


public:



	/////////////////////
	//Blocks          ///
	/////////////////////
	static Block* mBarrel; 
	static Block* mConductiveCable;
	static Block* mCrusher;
	static Block* mGrinder;
	static Block* mElectricFurnace;
	static Block* mPowerCell;
	static Block* mWoodenCrank;


	/////////////////////
	//Items           ///
	/////////////////////
	static Item* mCable;
	static Item* mIronDust;
	static Item* mWrench;
};