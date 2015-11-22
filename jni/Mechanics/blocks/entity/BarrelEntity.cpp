#include "BarrelEntity.h"

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

	//return;
	
	//if(compoundTag->tags["Item"] != nullptr)
	//	this->itemInstance = ItemInstance::fromTag(*((CompoundTag*)compoundTag->tags["Item"]));
	//
	//this->itemCount = ((IntTag*)compoundTag->tags["ItemCount"])->data;
	//this->isLocked = ((ByteTag*)compoundTag->tags["Locked"])->data == 0x01 ? true : false;
	//this->maxItems = ((IntTag*)compoundTag->tags["MaxItems"])->data;
}

bool BarrelEntity::save(CompoundTag& compoundTag)
{
	//TileEntity::save(compoundTag);

	//compoundTag->putInt("MaxItems", this->maxItems);
	//compoundTag->putInt("ItemCount", this->itemCount);
	//compoundTag->putByte("Locked", this->isLocked ? 0x01 : 0x00);
	
	//if(this->itemInstance != nullptr)
	//	compoundTag->putCompound("Item", std::unique_ptr<CompoundTag>(this->itemInstance->save()));

	return false;
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