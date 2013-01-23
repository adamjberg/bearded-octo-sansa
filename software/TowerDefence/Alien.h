/*
 * Alien.h
 *
 *  Created on: 2013-01-21
 *      Author: Danny
 */

#ifndef ALIEN_H_
#define ALIEN_H_
#include "Game.h"

struct Alien {
	struct Object* super;
	int frames, health;
};

struct Alien* initAlien(int, int, int, int);
void killAlien(struct Alien*);
#endif /* ALIEN_H_ */
