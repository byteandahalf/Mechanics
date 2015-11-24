#pragma once

#include "MCPE/world/item/ItemInstance.h"

class GrinderRecipe
{
private:
	ItemInstance* _inputItem;
	ItemInstance* _outputItem;
	int _neededTicks;

public:
	GrinderRecipe(ItemInstance* inputItem, ItemInstance* outputItem, int neededTicks);
	~GrinderRecipe();

	bool haveRequisite(ItemInstance* inputItem, int neededTicks);

	ItemInstance* getOutputItem() const;
};