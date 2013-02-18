/*
 * Alien.h
 *
 *  Created on: 2013-01-21
 *      Author: Danny
 *	Edited on: 2013-01-25
 *		Author: Perry
 */

#ifndef ALIEN_H_
#define ALIEN_H_

#include "Game.h"
#include "animation.h"

struct Alien {
	struct Object* super;
	char* name; // Unique identifier
	float current_life, max_life;
	float x_speed, y_speed;
	float speed, init_speed;
	int bounty; // money earned by killing this alien
	int score;
//	bool active;
	bool dead, isHit;
	bool towerBlock; // tower collision
	bool towerBlock_move_x; // true if the alien is supposed to move in x after colliding with tower range
	bool towerBlock_move_y; // true if the alien is supposed to move in x after colliding with tower range
	bool team_collide; // enemy collisions
	bool borderBlocked[4]; // border collisions : 0 = left, 1 = top, 2 = right, 3 = bottom

	int frame, scale;

	bool path_found;
	int path; // For finding the path

	struct Collidable* col;
};

struct Alien* initAlien(int, int, int, struct animation*, struct animation*, char*, float, float, int, struct Collidable*);
void resetAlien(struct Alien*, int, int);
void killAlien(void*);
//void explodeAlien(struct Collidable*, void*);

void alienAction(void* );
void alienCollide(void*, void*, struct Collidable*);

// float alien_getLife(void*);
// void checkAlienDead(void*, struct Collidable*);

void alienPath(struct Alien*);
void setAlienDirection(struct Alien*, int, int);
void followPath(struct Alien*);
#endif /* ALIEN_H_ */
