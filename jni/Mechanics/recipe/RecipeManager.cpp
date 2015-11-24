#include "RecipeManager.h"

RecipeManager::~RecipeManager()
{
	for (int i = 0; i < this->_grinderRecipes.size(); i++)
		delete this->_grinderRecipes[i];

	this->_grinderRecipes.clear();
}

void RecipeManager::addGrinderRecipe(GrinderRecipe* recipe)
{
	if(recipe != nullptr)
		this->_grinderRecipes.push_back(recipe);
}

ItemInstance* RecipeManager::getOutputItemFromGrinderRecipe(ItemInstance* inputItem, int neededTicks)
{
	for(int i = 0; i < this->_grinderRecipes.size(); i++)
	{
		if(this->_grinderRecipes[i]->haveRequisite(inputItem, neededTicks))
			return this->_grinderRecipes[i]->getOutputItem();
	}
	
	return nullptr;
}