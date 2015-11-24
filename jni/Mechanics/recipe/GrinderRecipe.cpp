#include "GrinderRecipe.h"

GrinderRecipe::GrinderRecipe(ItemInstance* inputItem, ItemInstance* outputItem, int neededTicks)
{
	this->_inputItem = inputItem;
	this->_outputItem = outputItem;
	this->_neededTicks = neededTicks;
}

GrinderRecipe::~GrinderRecipe()
{
	if(this->_inputItem != nullptr)
		delete this->_inputItem;

	if(this->_outputItem != nullptr)
		delete this->_outputItem;
}

bool GrinderRecipe::haveRequisite(ItemInstance* inputItem, int neededTicks)
{
	return ((this->_inputItem->sameItemAndAux(inputItem) && this->_neededTicks >= neededTicks) ? true : false);
}

ItemInstance* GrinderRecipe::getOutputItem() const
{
	return ItemInstance::clone(this->_outputItem);
}