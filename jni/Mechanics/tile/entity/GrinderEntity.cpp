#include "GrinderEntity.h"
#include "CTypes.h"

GrinderEntity::GrinderEntity(TilePos const& pos) : TileEntity(TileEntityType::Grinder, pos, "Grinder")
{}

void GrinderEntity::tick(TileSource*)
{
	//TODO
}

bool GrinderEntity::save(CompoundTag*)
{
	//TODO
	return false;
}