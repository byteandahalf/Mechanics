#include "Recipes.h"

Recipes::Type::Type(Block* block, char letter)
{
	this->item = nullptr;
	this->block = block;
	this->c = letter;
}

Recipes::Type::Type(Item* item, char letter)
{
	this->item = item;
	this->block = nullptr;
	this->c = letter;
}

Recipes::Type::Type(const ItemInstance& itemInstance, char letter)
{
	this->item = nullptr;
	this->block = nullptr;
	this->itemInstance = itemInstance;
	this->c = letter;
}