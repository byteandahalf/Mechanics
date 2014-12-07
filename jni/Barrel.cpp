#include "Barrel.h"

Barrel::Barrel(int id) : Tile(id, "cobblestone", &Material::wood)
{
	this->maxItems = 64 * 12;
	this->itemsCount = 0;
	this->itemID = 0;
}

void Barrel::use(Player* player, int x, int y, int z)
{
	Player_setDestPos(player, x + 1, y + 1, z);
}

std::string Barrel::getName() const
{
	return "Barrel";
}

std::string Barrel::getDescriptionId() const
{
	return "tile.Barrel.name";
}