#include "Barrel.h"

#include "MCPE/world/material/Material.h"

Barrel::Barrel(int blockId) : EntityBlock("barrel", blockId, "log", Material::getMaterial(MaterialType::WOOD)) 
{
	setDestroyTime(0.5);
	setExplodeable(3.0F);
	
	this->creativeCategory = 1;
	this->blockEntityType = BlockEntityType::Barrel;
}