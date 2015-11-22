#pragma once

#include <string>
#include <memory>

#include "BlockEntityType.h"
#include "MCPE/world/level/BlockPos.h"
#include "MCPE/world/level/BlockSource.h"
#include "MCPE/world/phys/AABB.h"
#include "MCPE/world/phys/Vec3.h"

class CompoundTag;
class Packet;
class BlockEntity
{
public:
	char filller1[28]; // WTF!!!! Another AABB??
	BlockPos position; // 40
	AABB hitbox; // 68
	int data; // 72
	BlockEntityType type; // 76
	int runningID; // 80
	bool clientSideOnly;
	BlockEntityRendererId rendererId; // 88
	bool haveChanged;


	BlockEntity(BlockEntityType type, BlockPos const& pos, std::string const& nameID);

	virtual ~BlockEntity();
	virtual void load(CompoundTag& tag);
	virtual bool save(CompoundTag& tag);
	virtual void tick(BlockSource& blockSource);
	virtual bool isFinished();
	virtual void onChanged(BlockSource& blockSource);
	virtual Packet* getUpdatePacket(BlockSource& blockSource);
	virtual void onUpdatePacket(CompoundTag& tag);
	virtual void onRemoved();
	virtual void triggerEvent(int, int);
	virtual void clearCache();
	virtual void onNeighborChanged(BlockSource& blockSource, int x, int y, int z);
	virtual void onNeighborChanged(BlockSource& blockSource, BlockPos const&);
	virtual float getShadowRadius(BlockSource& blockSource) const;
	virtual void* getCrackEntity(BlockSource& blockSource, BlockPos const&);

	void _destructionWobble(float&, float&, float&);
	void _resetAABB();
	float distanceToSqr(Vec3 const&);
	AABB getAABB() const;
	int getData() const;
	BlockPos& getPosition() const;
	int getRendererId() const;
	int getRunningId() const;
	int getType() const;
	bool isClientSideOnly() const;
	bool isInWorld() const;
	void setBB(AABB);
	void setChanged();
	void setClientSideOnly(bool);
	void setData(int);
	void setRendererId(BlockEntityRendererId);
	void setRunningId(int);
	void stopDestroy();

	static void setId(BlockEntityType, std::string const&);
	static bool isType(BlockEntity&, BlockEntityType);
	static void initBlockEntities();
	static BlockEntity* loadStatic(CompoundTag&);
};

class BlockEntityFactory
{
public:
	static std::unique_ptr<BlockEntity> createBlockEntity(BlockEntityType, const BlockPos&);
};