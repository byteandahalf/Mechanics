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

	this->itemInstance = new ItemInstance();
	this->itemInstance->setNull();
	this->isLocked = false;
	this->itemCount = 0;
	this->maxItems = 0;
}

void BarrelEntity::load(CompoundTag* compoundTag)
{	
	CompoundTag* item = (CompoundTag*)compoundTag->tags["Item"];
	this->itemInstance = new ItemInstance(((IntTag*)item->tags["ItemID"])->data, 1, ((IntTag*)item->tags["ItemDamage"])->data);
	this->itemCount = ((IntTag*)item->tags["ItemCount"])->data;

	this->isLocked = ((ByteTag*)compoundTag->tags["Locked"])->data == 0x01 ? true : false;
	this->maxItems = ((IntTag*)compoundTag->tags["MaxItems"])->data;
}

bool BarrelEntity::save(CompoundTag* compoundTag)
{
	TileEntity::save(compoundTag);

	CompoundTag* item = (CompoundTag*)Tag::newTag(Tag::TAG_Compound, "Item");
		item->putInt("ItemID", this->itemInstance->getId());
		item->putInt("ItemDamage", this->itemInstance->getAuxValue());
		item->putInt("ItemCount", this->itemCount);
	compoundTag->put("Item", item);

	compoundTag->putInt("MaxItems", this->maxItems);

	compoundTag->putByte("Locked", (char)this->isLocked);
	return true;
}


void BarrelEntity::clear()
{
	if(this->isLocked)
		this->itemCount = 0;
	else 
	{
		this->itemInstance->setNull();
		this->itemCount = 0;
		this->maxItems = 0;
	}
}
