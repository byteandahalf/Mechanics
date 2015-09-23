#include "GrinderTile.h"

GrinderTile::GrinderTile() : EntityTile(GRINDER_ID, "log", &Material::metal)
{
	this->category = 1;
	this->tileEntityType = TileEntityType::Grinder;

	this->init();
	this->setTicking(true);
	this->setNameId("grinder");
	this->setSoundType(Tile::SOUND_METAL);
	this->setDestroyTime(0.5);

	new TileItem(GRINDER_ID - 256);
}

int GrinderTile::getResource(Random*, int, int)
{
	return GRINDER_ID;
}

bool GrinderTile::use(Player*, int, int, int)
{
	return true;
}

void GrinderTile::tick(TileSource*, int, int, int, Random*)
{
	//TODO
}