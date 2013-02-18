/*
 * Collidable.h
 *
 *  Created on: 2013-01-27
 *      Author: Danny
 */

#ifndef COLLIDABLE_H_
#define COLLIDABLE_H_
#include "Game.h"

struct Collidable {
	struct Object** objCollection;
	int size;
	struct QuarTree* qt;
};

struct Collidable* initCollidable();
void addObjToCollide(struct Collidable*, struct Object*);
void checkCollision(struct Collidable*);
void removeFromCollide(struct Collidable*, struct Object*);
bool checkGoal(struct Collidable*, void*);
#endif /* COLLIDABLE_H_ */
