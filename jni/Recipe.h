#pragma once
#include <mcpe.h>
#include <vector>

class ItemPack
{
public:
	std::map<int, int> items;
};

class Recipe
{
public:
	ItemPack myItems;
};

class Recipes
{
public:

	std::vector<Recipe*> recipes;
	class Type
	{
	public:
		Item* item;
		Tile* tile;
		ItemInstance itemInstance;
		char c;
	};

	static Recipes* getInstance();
	void addShapedRecipe(std::vector<ItemInstance> const&, std::vector<std::string> const&, std::vector<Recipes::Type> const&);
};
