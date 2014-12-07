#ifndef PLAYER_H_
#define PLAYER_H_

#include <mcpe.h>

static ItemInstance* (*bl_Player_getCarriedItem)(Player*);
static void (*Player_setDestPos)(Player*, float, float, float);

#endif