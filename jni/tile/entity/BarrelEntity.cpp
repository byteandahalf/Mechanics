#include "BarrelEntity.h"
#include "../Barrel.h"
#include "Utils.h"

#include "mcpe/nbt/CompoundTag.h"
#include "mcpe/nbt/IntTag.h"
#include "mcpe/nbt/StringTag.h"
#include "mcpe/nbt/ByteTag.h"

BarrelEntity::BarrelEntity(const TilePos& pos) : TileEntity(TileEntityType::Barrel, pos, BARREL_STRING_ID)
{
	this->tile = Tile::tiles[Barrel::BARREL_ID];
	this->rendererId = TileEntityRendererId::TR_DEFAULT_RENDERER;

	this->itemInstance = NULL;
	this->isLocked = false;
	this->itemCount = 0;
	this->maxItems = 0;
}

void BarrelEntity::load(CompoundTag* compoundTag)
{	
	if(compoundTag->tags["Item"] != NULL)
		this->itemInstance = ItemInstance::fromTag((CompoundTag*)compoundTag->tags["Item"]);
	
	this->itemCount = ((IntTag*)compoundTag->tags["ItemCount"])->data;
	this->isLocked = ((ByteTag*)compoundTag->tags["Locked"])->data == 0x01 ? true : false;
	this->maxItems = ((IntTag*)compoundTag->tags["MaxItems"])->data;
}

bool BarrelEntity::save(CompoundTag* compoundTag)
{
	TileEntity::save(compoundTag);

	compoundTag->putInt("MaxItems", this->maxItems);
	compoundTag->putInt("ItemCount", this->itemCount);
	LOGI("Count: %d", this->itemCount);
	compoundTag->putByte("Locked", this->isLocked ? 0x01 : 0x00);
	
	if(this->itemInstance != NULL)
		compoundTag->put("Item", this->itemInstance->save((CompoundTag*)Tag::newTag(Tag::TAG_Compound, "Item")));

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
