#include "BarrelEntity.h"

#include "MCPE/nbt/CompoundTag.h"

BarrelEntity::BarrelEntity(const BlockPos& pos) : BlockEntity(BlockEntityType::Barrel, pos, BARREL_STRING_ID)
{
	//this->rendererId = 0;

	this->itemInstance = nullptr;
	this->isLocked = false;
	this->itemCount = 0;
	this->maxItems = 0;
}

BarrelEntity::~BarrelEntity()
{
	if(this->itemInstance != nullptr)
		delete this->itemInstance;
}

void BarrelEntity::load(CompoundTag& compoundTag)
{
	if(compoundTag.contains("ItemID"))
		this->itemInstance = new ItemInstance(compoundTag.getInt("ItemID"), 1, compoundTag.getInt("ItemAux"));

	this->itemCount = compoundTag.getInt("ItemCount");
	
	this->maxItems = compoundTag.getInt("MaxItems");
	this->isLocked = compoundTag.getBoolean("Locked");
}

bool BarrelEntity::save(CompoundTag& compoundTag)
{
	BlockEntity::save(compoundTag);
	
	if(this->itemInstance != nullptr) 
	{
		compoundTag.putInt("ItemID", this->itemInstance->getId());
		compoundTag.putInt("ItemAux", this->itemInstance->getAuxValue());
	}

	compoundTag.putInt("ItemCount", this->itemCount);

	compoundTag.putInt("MaxItems", this->maxItems);
	compoundTag.putBoolean("Locked", this->isLocked);

	return true;
}


void BarrelEntity::clear()
{
	if(this->isLocked)
		this->itemCount = 0;
	else 
	{	
		if(this->itemInstance != nullptr)
			delete this->itemInstance;

		this->itemInstance = nullptr;
		this->itemCount = 0;
		this->maxItems = 0;
	}
	this->setChanged();
}