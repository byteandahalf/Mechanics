#pragma once

#include "MCPE/inventory/FillingContainer.h"

class Block;
class Inventory : public FillingContainer
{
public:
	char filler[0x20];
	int selectedSlot;

	
	Inventory(Player*);

	virtual ~Inventory();
	virtual int getContainerSize() const;
	virtual bool add(ItemInstance&);
	virtual bool canAdd(const ItemInstance&) const;
	virtual void clearSlot(int);
	virtual void doDrop(ItemInstance&, bool);
	virtual int getEmptySlotsCount() const;
	virtual void setContainerSize(int);

	void canDestroy(Block&);
	void clearInventoryWithDefault(bool);
	void getAttackDamage();
	void getDestroySpeed(Block&);
	int getLinkedSlotForExactItem(ItemInstance const&) const;
	int getLinkedSlotForItem(int);
	int getLinkedSlotForItemIdAndAux(int, int);
	ItemInstance* getSelectedItem() const;
	int getSelectedSlot() const;
	int getSelectionSize();
	void moveToEmptySelectionSlot(int);
	void moveToSelectedSlot(int);
	void moveToSelectionSlot(int, int);
	void removeItemInstance(ItemInstance const*);
	void selectSlot(int);
	void setupDefault();
};