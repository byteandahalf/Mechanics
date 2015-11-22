#pragma once

#include "Container.h"

class ListTag;
class FillingContainer : public Container
{
public:
	virtual ~FillingContainer();

	virtual ItemInstance* getItem(int) const;
	virtual void setItem(int, ItemInstance*);
	virtual void removeItem(int, int);
	virtual std::string getName();
	virtual int getContainerSize() const = 0;
	virtual int getMaxStackSize() const;
	virtual void startOpen(Player&);
	virtual void stopOpen(Player&);

	virtual bool add(ItemInstance&);
	virtual bool canAdd(const ItemInstance&) const;
	virtual void clearSlot(int);
	virtual void doDrop(ItemInstance&, bool);
	virtual int getEmptySlotsCount() const;

	void _addResource(ItemInstance const&);
	void _fixBackwardCompabilityItem(ItemInstance&);
	int _getFreeSlot() const;
	ItemInstance* _getSlot(int);
	ItemInstance* _getSlot(int, int);
	int _getSlotWithRemainingSpace(ItemInstance const&);
	bool _isCreative();
	void _release(int);
	void clearInventory(int);
	void compressLinkedSlotList(int);
	bool contains(ItemInstance&);
	void dropAll(bool);
	void dropSlot(int, bool, bool, bool);
	ItemInstance* getLinked(int);
	int getLinkedSlot(int);
	void getLinkedSlots();
	int getLinkedSlotsCount();
	int getSlotWithItem(ItemInstance const&, bool, bool);
	void hasResource(int);
	void linkEmptySlot(int);
	void linkSlot(int, int);
	void load(ListTag const&);
	void removeResource(ItemInstance const&);
	void removeResource(ItemInstance const&, bool);
	void removeResource(int);
	void replace(std::vector<ItemInstance>, int);
	void replaceSlot(int, ItemInstance&);
	CompoundTag* save();
	void setContainerChanged();
	void swapSlots(int, int);
	void unlinkSlot(int);
};