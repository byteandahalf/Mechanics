#pragma once

#include <string>
#include <vector>

#include "MCPE/world/item/ItemInstance.h"

class Player;
class Container
{
public:
	~Container();

	virtual ItemInstance* getItem(int) const = 0;
	virtual void setItem(int, ItemInstance*) = 0;
	virtual void removeItem(int, int) = 0;
	virtual std::string getName() = 0;
	virtual int getContainerSize() const = 0;
	virtual int getMaxStackSize() const = 0;
	virtual void startOpen(Player&) = 0;
	virtual void stopOpen(Player&) = 0;

	virtual std::vector<ItemInstance*> getSlotCopies() const;
	virtual std::vector<ItemInstance*> getSlots();
	virtual int getItemCount(int);
};