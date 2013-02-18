/*
 * Object.h
 *
 *  Created on: 2013-01-17
 *      Author: Danny
 */

#ifndef OBJECT_H_
#define OBJECT_H_
#include "Game.h"

struct Cursor;
struct Range;
struct Collidable;
enum Type{dummy = 0, ship = 6, bullet = 1, alien = 2, panel = 3, cursor = 4, button = 5};
struct Object {
	int x, y, scale, prev_x, prev_y, center_x, center_y, total_stats, collideRange, animate_prev_x, animate_prev_y;
	int isBound;
	struct animation* currImg;
	struct animation* stats[3];
	struct Range* r;
	enum Type class;
	int isSelected, end_frame;
	int startAnimated, isShowing;
	int curr_frame;
	int isRunable;
	void* subObject;
	void (*animate)(struct animation**, int, int, int, int, struct Object*, struct Cursor*);
	void (*collide)(void*, void*, struct Collidable*);
	void (*action)(void*);
};

struct Object* initObject(int, int, int, struct animation*, void*);
void setXY(struct Object*, int, int);
void setXY_noBound(struct Object*, int, int);
void killObject(struct Object*);
void setStats(struct Object*, int);
void addStats(struct Object*, struct animation*);
void dummyCollide(void*, void*, struct Collidable*);
void dummyAction(void*);
void setCollideRagne(struct Object*, int);
#endif /* OBJECT_H_ */
