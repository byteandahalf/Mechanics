#include "WoodenGearItem.h"

WoodenGearItem::WoodenGearItem(short itemId) : Item("woodenGear", itemId)
{
	this->creativeCategory = 1;

	this->setIcon("apple", 0);
	this->setMaxStackSize(16);
}