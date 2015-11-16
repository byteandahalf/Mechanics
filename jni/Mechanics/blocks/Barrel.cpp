#include "Barrel.h"

Barrel::Barrel(int blockId) : EntityBlock("barrel", blockId, "stone", Material::mMaterials[3]) 
{
	setDestroyTime(1.0F);
	setExplodeable(15.0F);
	
	this->creativeCategory = 1;
	this->blockEntityType = BlockEntityType::Barrel;
}