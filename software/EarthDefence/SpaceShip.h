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
	int damage, price;
	int fire_rate;
	struct BulletCollection* weapon;
	unsigned int frame;
	int level, type;
	char* name;
	void (*ss_shoot)(int, int, float, float, struct SpaceShip*);
};

struct SpaceShip* initSpaceShip(int, int, int, struct animation*);
void setShipProperty(struct SpaceShip*, int);
void killSpaceShip(void*);
void spaceShipShoot(int, int, float, float, struct SpaceShip*);
void spaceShipCollide(void*, void*, struct Collidable*);
void setWeapon(struct BulletCollection*, struct SpaceShip*);
void shipAction(void*);
void upgrade(struct SpaceShip*);
#endif /* SPACESHIP_H_ */
