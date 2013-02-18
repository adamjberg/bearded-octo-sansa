/*
 * Bullet.h
 *
 *  Created on: 2013-01-24
 *      Author: Diana
 *  Edited on: 2013-01-25
 * 	Author: Perry
 */

#ifndef BULLET_H_
#define BULLET_H_
#include "Game.h"

struct animation;

struct Bullet {
	struct Object* super;
	float v_x, v_y;
	float acc_x, acc_y;
	float damage;
	float next_x, next_y; // for detecting location
	float enemy_v_x, enemy_v_y;
	int speed_adj;
	int direction_x, direction_y;
	int init_x, init_y;
	int frame, image_changed_flag;
	struct BulletCollection* pBulCollection;

	void (*kill)(struct Bullet*);
};

struct Bullet* initBullet(int, int, int, float , struct animation*, struct BulletCollection*);
void setupBullet(struct Bullet*, struct animation*, void*, void*);
void killBullet(void*);
void shootEnemy(struct Bullet*);
void setBulletInitPosition(int, int, struct Bullet*);
void bulletAction(void*);
void bulletCollide(void*, void*, struct Collidable*);
void blackHoleAction(void*);
void blackHoleCollide(void*, void*, struct Collidable*);
void laserAction(void*);
void laserCollide(void*, void*, struct Collidable*);
void laserAnimate(struct animation**, int, int, int, int, struct Object*, struct Cursor*);
void resetBullet(struct Bullet*, int);
void fireAction(void*);
void fireCollide(void*, void*, struct Collidable*);
void freezeAction(void*);
void freezeCollide(void*, void*, struct Collidable*);
#endif
