#include "IronDustItem.h"

IronDustItem::IronDustItem(short itemId) : Item("ironDust", itemId)
{
	this->creativeCategory = 1;
	
	this->setIcon("ironDust", 0);
	this->setMaxStackSize(64);
}