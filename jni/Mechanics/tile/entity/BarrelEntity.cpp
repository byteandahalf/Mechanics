#include "BarrelEntity.h"
#include "../BarrelTile.h"

#include "CTypes.h"

#include "MCPE/nbt/CompoundTag.h"
#include "MCPE/nbt/IntTag.h"
#include "MCPE/nbt/StringTag.h"
#include "MCPE/nbt/ByteTag.h"

BarrelEntity::BarrelEntity(const TilePos& pos) : TileEntity(TileEntityType::Barrel, pos, BARREL_STRING_ID)
{
	this->rendererId = 0;
	this->tile = Tile::tiles[BarrelTile::BARREL_ID];

	this->itemInstance = NULL;
	this->isLocked = false;
	this->itemCount = 0;
	this->maxItems = 0;
}

void BarrelEntity::load(CompoundTag* compoundTag)
{
	return;
	
	if(compoundTag->tags["Item"] != NULL)
		this->itemInstance = ItemInstance::fromTag(*((CompoundTag*)compoundTag->tags["Item"]));
	
	this->itemCount = ((IntTag*)compoundTag->tags["ItemCount"])->data;
	this->isLocked = ((ByteTag*)compoundTag->tags["Locked"])->data == 0x01 ? true : false;
	this->maxItems = ((IntTag*)compoundTag->tags["MaxItems"])->data;
}

bool BarrelEntity::save(CompoundTag* compoundTag)
{
	TileEntity::save(compoundTag);

	compoundTag->putInt("MaxItems", this->maxItems);
	compoundTag->putInt("ItemCount", this->itemCount);
	compoundTag->putByte("Locked", this->isLocked ? 0x01 : 0x00);
	
	if(this->itemInstance != NULL)
		compoundTag->putCompound("Item", std::unique_ptr<CompoundTag>(this->itemInstance->save()));

	return true;
}


void BarrelEntity::clear()
{
	if(this->isLocked)
		this->itemCount = 0;
	else 
	{
		this->itemInstance = NULL;
		this->itemCount = 0;
		this->maxItems = 0;
	}
	this->setChanged();
}