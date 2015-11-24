#include "Mechanics.h"

#include "MCPE/world/item/BlockItem.h"
#include "MCPE/world/recipes/FurnaceRecipes.h"
#include "Mechanics/blocks/entity/BarrelEntity.h"
#include "Mechanics/blocks/entity/GrinderEntity.h"
#include "Mechanics/blocks/BarrelBlock.h"
#include "Mechanics/blocks/GrinderBlock.h"
#include "Mechanics/blocks/WoodenCrankBlock.h"
#include "Mechanics/items/IronDustItem.h"
#include "Mechanics/items/GoldDustItem.h"

Block* Mechanics::mBarrel; 
Block* Mechanics::mConductiveCable;
Block* Mechanics::mCrusher;
Block* Mechanics::mElectricFurnace;
Block* Mechanics::mGrinder;
Block* Mechanics::mPowerCell;
Block* Mechanics::mWoodenCrank;

Item* Mechanics::mCable;
Item* Mechanics::mGoldDust;
Item* Mechanics::mIronDust;
Item* Mechanics::mWrench;


Mechanics::Mechanics()
{
	this->_recipesManager = new RecipeManager();
}

void Mechanics::initBlocks()
{
	//Init block
	Block::mBlocks[201] = mBarrel = (new BarrelBlock(201))->init();
	Block::mBlocks[202] = mGrinder = (new GrinderBlock(202))->init();
	Block::mBlocks[203] = mWoodenCrank = (new WoodenCrankBlock(203))->init();
	//Block::mBlocks[201] = mConductiveCable = nullptr;
	//Block::mBlocks[201] = mCrusher = nullptr;
	//Block::mBlocks[201] = mElectricFurnace = nullptr;
	//Block::mBlocks[201] = mPowerCell = nullptr;
}

void Mechanics::initItems()
{
	//Init Item
	//Item::mItems[300] = mCable =  nullptr;
	Item::mItems[460] = mIronDust = new IronDustItem(460);
	Item::mItems[461] = mGoldDust = new GoldDustItem(461);
	//Item::mItems[300] = mWrench = nullptr;
}


void Mechanics::initBlockItems()
{
	Item::mItems[mBarrel->blockId] = new BlockItem(mBarrel->getDescriptionId(), mBarrel->blockId - 0x100);
	Item::mItems[mGrinder->blockId] = new BlockItem(mGrinder->getDescriptionId(), mGrinder->blockId - 0x100);
	Item::mItems[mWoodenCrank->blockId] = new BlockItem(mWoodenCrank->getDescriptionId(), mWoodenCrank->blockId - 0x100);
}

void Mechanics::initCreativeItems()
{
	//Blocks
	Item::addCreativeItem(mBarrel, 0);
	Item::addCreativeItem(mGrinder, 0);
	Item::addCreativeItem(mWoodenCrank, 0);

	//Items
	//Item::addCreativeItem(mIronDust, 0);
	//Item::addCreativeItem(mGoldDust, 0);
}

void Mechanics::initRecipies()
{
	//Grinder Recipes
	this->_recipesManager->addGrinderRecipe(new GrinderRecipe(new ItemInstance(14, 1, 0), new ItemInstance(461, 2, 0), 20)); // Gold Ore -> Gold Dust x2
	this->_recipesManager->addGrinderRecipe(new GrinderRecipe(new ItemInstance(15, 1, 0), new ItemInstance(460, 2, 0), 20)); // Iron Ore -> Iron Dust x2
	this->_recipesManager->addGrinderRecipe(new GrinderRecipe(new ItemInstance(16, 1, 0), new ItemInstance(263, 6, 0), 20)); // Coal Ore -> Coal x6
	this->_recipesManager->addGrinderRecipe(new GrinderRecipe(new ItemInstance(56, 1, 0), new ItemInstance(264, 3, 0), 20)); // Diamond Ore -> Diamond x3
	this->_recipesManager->addGrinderRecipe(new GrinderRecipe(new ItemInstance(73, 1, 0), new ItemInstance(331, 5, 0), 20)); // Redstone Ore -> Redstone Dust x5
	this->_recipesManager->addGrinderRecipe(new GrinderRecipe(new ItemInstance(4, 1, 0), new ItemInstance(13, 1, 0), 10)); // Cobblestone -> Gravel
	this->_recipesManager->addGrinderRecipe(new GrinderRecipe(new ItemInstance(318, 1, 0), new ItemInstance(289, 1, 0), 10)); // Flint -> Gunpowder


	//Furnace Recipes
	FurnaceRecipes::getInstance()->addFurnaceRecipe(460, ItemInstance(265, 1, 0)); // Iron Dust -> Iron Ingot
	FurnaceRecipes::getInstance()->addFurnaceRecipe(461, ItemInstance(266, 1, 0)); // Gold Dust -> Gold Ingot

	//Crafting Table Recipes
	//TODO: Add Barrel Recipe
	//TODO: Add Grinder Recipe
}

void Mechanics::initBlockEntities()
{
	BlockEntity::setId(BlockEntityType::Barrel, BARREL_STRING_ID);
	BlockEntity::setId(BlockEntityType::Grinder, GRINDER_STRING_ID);
}

RecipeManager* Mechanics::getRecipeManager() const
{
	return this->_recipesManager;
}