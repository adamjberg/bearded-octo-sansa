/*
 * Object.c
 *
 *  Created on: 2013-01-17
 *      Author: Danny
 */

#include "Object.h"

/*
 * Constructor of Object which is a super class of all objects in the game
 */
struct Object* initObject(int pos_x, int pos_y, int size, struct animation* s1) {
	struct Object* this = (struct Object*)malloc(sizeof(struct Object));
	this->x = pos_x;
	this->y = pos_y;
	this->prev_x = pos_x;
	this->prev_y = pos_y;
	this->center_x = pos_x + size/2;
	this->center_y = pos_y + size/2;
	this->scale = size;
	this->currImg = s1;
	this->stats[0] = s1;
	this->total_stats = 1;
	this->isSelected = 0;
	this->animate = animated;
	return this;
}

/*
 * destructor of object
 */
void killObject(struct Object* this) {
	int i;
	this->currImg = NULL;
	this->animate = NULL;
	for(i = 0; i < this->total_stats; i++) {
		killAnimation(this->stats[i], this->scale);
	}
	cleanDrawing(pixel_buffer, this->x, this->y, this->scale);
	free(this);
}

/*
 * stats in an object define the animations that it is allow to run
 * this function will change the animation of an object
 */
void setStats(struct Object* this, int stat) {
	this->currImg = this->stats[stat];
}
void addStats(struct Object* this, struct animation* s) {
	this->stats[this->total_stats] = s;
	this->total_stats++;
}

/*
 * change the coordinate system of an object
 */
void setXY(struct Object* o, int pos_x, int pos_y) {
	o->prev_x = o->x;
	o->prev_y = o->y;
	o->x = pos_x;
	o->y = pos_y;
	o->center_x = pos_x + o->scale/2;
	o->center_y = pos_y + o->scale/2;
}
