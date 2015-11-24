#pragma once

#include <vector>

#include "GrinderRecipe.h"

class RecipeManager
{
private:
	std::vector<GrinderRecipe*> _grinderRecipes;

public:

	~RecipeManager();

	void addGrinderRecipe(GrinderRecipe* recipe);

	ItemInstance* getOutputItemFromGrinderRecipe(ItemInstance* inputItem, int neededTicks);
};