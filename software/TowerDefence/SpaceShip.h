/*
 * SpaceShip.h
 *
 *  Created on: 2013-01-21
 *      Author: Danny
 */

#ifndef SPACESHIP_H_
#define SPACESHIP_H_
#include "Game.h"

struct SpaceShip {
	struct Object* super;
	int damage, level;
};

struct SpaceShip* initSpaceShip(int, int, int, int);
void killSpaceShiop(struct SpaceShip*);
#endif /* SPACESHIP_H_ */
