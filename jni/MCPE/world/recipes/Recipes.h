#pragma once 

#include <string>
#include <vector>

#include "MCPE/world/item/ItemInstance.h"

class Block;
class Item;
class Recipes {

public:
	static Recipes* getInstance();
	void* getRecipes();
	static void teardownRecipes();

	class Type 
	{
	public:
		Type(Item*, char);
		Type(Block*, char);
		Type(const ItemInstance&, char);

		Item* item;
		Block* block;
		ItemInstance itemInstance;
		char c;
	};

	static std::vector<std::string> Shape(const std::string&);
	static std::vector<std::string> Shape(const std::string&, const std::string&);
	static std::vector<std::string> Shape(const std::string&, const std::string&, const std::string&);


	void addShapedRecipe(const ItemInstance&, const std::string&, const std::vector<Recipes::Type>&);
	void addShapedRecipe(const ItemInstance&, const std::string&, const std::string&, const std::vector<Recipes::Type>&);
	void addShapedRecipe(const ItemInstance&, const std::string&, const std::string&, const std::string&, const std::vector<Recipes::Type>&);

	void addShapedRecipe(const ItemInstance&, const std::vector<std::string>&, const std::vector<Recipes::Type>&);
	void addSingleIngredientRecipeItem(const ItemInstance&, const ItemInstance&);


	//void addShapedRecipe(const Recipe::ResultList&, const std::vector<std::string>&, const std::vector<Recipes::Type>&);

private:
	Recipes();

	void addStairRecipies();

	void _addWoodStairsRecipes();

	void _addStoneStairsRecipes();

	void _addFenceAndGateRecipes();

	void _addGateRecipes();

	void _addFenceRecipes();

	~Recipes();

	static Recipes* instance;
};
