#include "GrinderEntity.h"
#include "Common.h"
#include "MCPE/nbt/CompoundTag.h"
#include "MCPE/world/level/Level.h"
#include "MCPE/world/entity/ItemEntity.h"

GrinderEntity::GrinderEntity(const BlockPos& pos) : BlockEntity(BlockEntityType::Grinder, pos, GRINDER_STRING_ID)
{
	this->inputItem = nullptr;
	this->outputItem = nullptr;
	this->tickCount = 0;
}

GrinderEntity::~GrinderEntity()
{
	if(this->inputItem == nullptr)
		delete this->inputItem;

	if(this->outputItem == nullptr)
		delete this->outputItem;
}

void GrinderEntity::tick(BlockSource& blockSource)
{

	if(this->inputItem != nullptr && this->tickCount >= 0)
	{
		ItemInstance* recipeStack = mechanics->getRecipeManager()->getOutputItemFromGrinderRecipe(this->inputItem, this->tickCount); 
		if(recipeStack == nullptr)
			return;

		LOGI("#1.1");
		if(this->outputItem != nullptr && this->outputItem->sameItemAndAux(recipeStack))
		{
			LOGI("#1.1.1");
			this->outputItem->count += recipeStack->count;
			if(--this->inputItem->count == 0)
				delete this->inputItem;

			this->tickCount = 0;
			delete recipeStack;
		}
		else if(this->outputItem == nullptr)
		{
			LOGI("#1.1.2");
			//this->outputItem = recipeStack;

			BlockPos pos = this->getPosition();
			pos.y--;

			blockSource.getLevel()->addEntity(std::unique_ptr<Entity>(new ItemEntity(blockSource, Vec3(pos), recipeStack, 32)));

			if(--this->inputItem->count == 0)
				delete this->inputItem;

			this->tickCount = 0;
		}
		else
			delete recipeStack;
	}
}

void GrinderEntity::load(CompoundTag& tag)
{
	if(tag.contains("InputItemID")) //Load input item
		this->inputItem = new ItemInstance(tag.getInt("InputItemID"), tag.getInt("InputItemCount"), tag.getInt("InputItemAUX"));

	if(tag.contains("OutputItemID")) //Load output item
		this->inputItem = new ItemInstance(tag.getInt("OutputItemID"), tag.getInt("OutputItemCount"), tag.getInt("OutputItemAUX"));

	//Load tick count
	this->tickCount = tag.getInt("TickCount");
}

bool GrinderEntity::save(CompoundTag& tag)
{
	//Save BlockEntity ID and Position
	BlockEntity::save(tag);

	if(this->inputItem != nullptr)
	{
		//Save input item
		tag.putInt("InputItemID", this->inputItem->getId());
		tag.putInt("InputItemAUX", this->inputItem->getAuxValue());
		tag.putInt("InputItemCount", this->inputItem->count);
	}

	if(this->outputItem != nullptr)
	{	
		//Save output item
		tag.putInt("OutputItemID", this->outputItem->getId());
		tag.putInt("OutputItemAUX", this->outputItem->getAuxValue());
		tag.putInt("OutputItemCount", this->outputItem->count);
	}

	//Save tick count
	tag.putInt("TickCount", this->tickCount);

	//Save the BlockEntity to level
	return true;
}

