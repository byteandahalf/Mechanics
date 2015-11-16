#include "Mechanics.h"

#include "MCPE/world/item/BlockItem.h"
#include "Mechanics/blocks/Barrel.h"

Block* Mechanics::mBarrel; 
Block* Mechanics::mConductiveCable;
Block* Mechanics::mCrusher;
Block* Mechanics::mElectricFurnace;
Block* Mechanics::mGrinder;
Block* Mechanics::mPowerCell;

Item* Mechanics::mCable;
Item* Mechanics::mOreDust;
Item* Mechanics::mWrench;


void Mechanics::initBlocks()
{
	//Init block
	Block::mBlocks[201] = mBarrel = (new Barrel(201))->init(); 
	//Block::mBlocks[201] = mConductiveCable = nullptr;
	//Block::mBlocks[201] = mCrusher = nullptr;
	//Block::mBlocks[201] = mElectricFurnace = nullptr;
	//Block::mBlocks[201] = mGrinder =  nullptr;
	//Block::mBlocks[201] = mPowerCell = nullptr;
}

void Mechanics::initItems()
{
	//Init Item
	//Item::mItems[300] = mCable =  nullptr;
	//Item::mItems[300] = mOreDust = nullptr;
	//Item::mItems[300] = mWrench = nullptr;
}


void Mechanics::addBlockItems()
{
	Item::mItems[mBarrel->blockId] = new BlockItem(mBarrel->getDescriptionId(), mBarrel->blockId - 0x100);
}

void Mechanics::initCreativeItems()
{
	Item::addCreativeItem(mBarrel, 0);
}

void Mechanics::initRecipies()
{
	//TODO: Add recipe for the block and item
}

void Mechanics::initBlockEntities()
{
	BlockEntity::setId(BlockEntityType::Barrel, "Barrel");
}