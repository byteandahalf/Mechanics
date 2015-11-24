#pragma once

class FurnaceRecipes 
{
public:

	FurnaceRecipes(void);

	static FurnaceRecipes* getInstance();

	void addFurnaceRecipe(int, ItemInstance const&);
	void addFurnaceRecipeAuxData(short,short, ItemInstance const&);
	void clearFurnaceRecipes();
	void getResult(ItemInstance const*);
	void isFurnaceItem(ItemInstance*);
	void teardownFurnaceRecipes();
};