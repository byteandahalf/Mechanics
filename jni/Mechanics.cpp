#include "Mechanics.h"

#include "MCPE/world/item/BlockItem.h"
#include "MCPE/world/recipes/FurnaceRecipes.h"
#include "Mechanics/blocks/entity/BarrelEntity.h"
#include "Mechanics/blocks/entity/GrinderEntity.h"
#include "Mechanics/blocks/BarrelBlock.h"
#include "Mechanics/blocks/GrinderBlock.h"
#include "Mechanics/blocks/WoodenCrankBlock.h"
#include "Mechanics/items/IronDustItem.h"

Block* Mechanics::mBarrel; 
Block* Mechanics::mConductiveCable;
Block* Mechanics::mCrusher;
Block* Mechanics::mElectricFurnace;
Block* Mechanics::mGrinder;
Block* Mechanics::mPowerCell;
Block* Mechanics::mWoodenCrank;

Item* Mechanics::mCable;
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
	Item::mItems[300] = mIronDust = new IronDustItem(300);
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
	Item::addCreativeItem(mIronDust, 0);
}

void Mechanics::initRecipies()
{
	//Grinder Recipes
	this->_recipesManager->addGrinderRecipe(new GrinderRecipe(new ItemInstance(14, 1, 0), new ItemInstance(266, 2, 0), 20)); // TODO: Implement Ore dust...

	//Furnace Recipes
	FurnaceRecipes::getInstance()->addFurnaceRecipe(201, ItemInstance(1, 4, 0));

	//Crafting Table Recipes
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