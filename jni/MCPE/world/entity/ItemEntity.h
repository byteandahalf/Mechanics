#pragma once

#include "Entity.h"

class ItemEntity : public Entity
{
public:
	char filler[100];//44

	ItemEntity(BlockSource&);
	ItemEntity(BlockSource&, Vec3 const&, ItemInstance const&, int);

	virtual ~ItemEntity();
	virtual void* getAddPacket();
	virtual void normalTick();
	virtual void playerTouch(Player&);
	virtual	bool isPushable();
	virtual void hurt(EntityDamageSource const&, int);
	virtual EntityType getEntityTypeId() const;
	virtual void getSourceUniqueID();
	virtual AABB getHandleWaterAABB();
	virtual void burn(int);
	virtual void readAdditionalSaveData(CompoundTag const&);
	virtual void addAdditionalSaveData(CompoundTag &);

	void _defineEntityData();
	void _validateItem();
	void setSourceEntity(Entity const*);
};