#include "GoldDustItem.h"

GoldDustItem::GoldDustItem(short itemId) : Item("goldDust", itemId)
{
	this->creativeCategory = 1;
	
	this->setIcon("goldDust", 0);
	this->setMaxStackSize(64);
}