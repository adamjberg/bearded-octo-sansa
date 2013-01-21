/*
 * Object.h
 *
 *  Created on: 2013-01-17
 *      Author: Danny
 */

#ifndef OBJECT_H_
#define OBJECT_H_
#include "animation.h"
#include <stdlib.h>
#include "Game.h"

enum Type{tower = 0, bullet = 1, alien = 2, panel = 3};
struct Object {
	int x, y, scale, prev_x, prev_y, center_x, center_y, total_stats;
	struct animation *currImg;
	struct animation* stats[3];
	enum Type class;
	int isSelected;
	void (*animate)(alt_up_pixel_buffer_dma_dev*, struct animation**, int, int, int, int);
	void (*collide)(struct Object*, struct Object*);
};

struct Object* initObject(int, int, int, struct animation*);
void setXY(struct Object*, int, int);
void killObject(struct Object*);
void setStats(struct Object*, int);
void addStats(struct Object*, struct animation*);
#endif /* OBJECT_H_ */
