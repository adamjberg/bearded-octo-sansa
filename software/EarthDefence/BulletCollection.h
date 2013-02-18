/*
 * BulletCollection.h
 *
 *  Created on: 2013-01-26
 *      Author: Diana
 */


#ifndef BULLETCOLLECTION_H_
#define BULLETCOLLECTION_H_
#include "Game.h"

struct SpaceShip;
struct BulletCollection{
	struct Object* super;
	struct Bullet* bullets[50];
	struct Env* env;
	struct Collidable* collision_checker;
	int pStart, pEnd, total_bullet_available;

	void (*bc_fire) (int, int, struct BulletCollection*, struct SpaceShip*); // pointer to function
};

struct BulletCollection* initBulletCollection(struct Env*, struct Collidable*);
void killBulletCollection(void*);
void updatePositionBullets(int, int, struct BulletCollection*);
void setBulletAcceleration(int, int, struct Bullet*);
void setBulletVelocity(int, int, struct Bullet*);
void fire_tower1_level1(int, int, struct BulletCollection*, struct SpaceShip*);
void fire_tower1_level2(int, int, struct BulletCollection*, struct SpaceShip*);
void fire_tower2_level1(int, int, struct BulletCollection*, struct SpaceShip*);
void fire_tower2_level2(int, int, struct BulletCollection*, struct SpaceShip*);
void fire_tower3_level1(int, int, struct BulletCollection*, struct SpaceShip*);
void fire_tower3_level2(int, int, struct BulletCollection*, struct SpaceShip*);
void fire_tower4_level1(int, int, struct BulletCollection*, struct SpaceShip*);
void fire_tower4_level2(int, int, struct BulletCollection*, struct SpaceShip*);
void fire_tower5_level1(int, int, struct BulletCollection*, struct SpaceShip*);
void fire_tower5_level2(int, int, struct BulletCollection*, struct SpaceShip*);
void initBulletToWeapon1(int, struct animation*, struct animation*, struct BulletCollection*);
void initBulletToWeapon2(int, struct animation*, struct animation*, struct BulletCollection*);
void initBulletToWeapon3(int, struct animation*, struct animation*, struct BulletCollection*);
void initBulletToWeapon4(int, struct animation*, struct animation*, struct BulletCollection*);
void initBulletToWeapon5(int, struct animation*, struct animation*, struct BulletCollection*);
bool launchMainBullet(struct BulletCollection*, struct SpaceShip*);
bool launchSecondaryBullet(struct BulletCollection*, struct SpaceShip*);
void nextBullet(struct BulletCollection*);
void reloadBullet(struct BulletCollection*, struct Bullet*, int);
#endif
