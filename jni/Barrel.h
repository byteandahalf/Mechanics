#ifndef BARREL_H_
#define BARREL_H_

#include <mcpe.h>
#include "Player.h"

class Barrel : public Tile
{
private:
	int itemID;
	int maxItems;
	int itemsCount;

public:
	Barrel(int id);

	virtual void use(Player*, int, int, int);

	virtual std::string getName() const;
	virtual std::string getDescriptionId() const;
};
#endif