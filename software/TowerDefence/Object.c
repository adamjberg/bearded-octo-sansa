/*
 * Object.c
 *
 *  Created on: 2013-01-17
 *      Author: Danny
 */

#include "Object.h"

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
	this->animate = animated;
	return this;
}

void killObject(struct Object* this) {
	int i;
	this->currImg = NULL;
	this->animate = NULL;
	for(i = 0; i < this->total_stats; i++) {
		killAnimation(this->stats[i], this->scale);
	} free(this);
}

void setStats(struct Object* this, int stat) {
	this->currImg = this->stats[stat];
}
void addStats(struct Object* this, struct animation* s) {
	this->stats[this->total_stats] = s;
	this->total_stats++;
}

void setXY(struct Object* o, int pos_x, int pos_y) {
	o->prev_x = o->x;
	o->prev_y = o->y;
	o->x = pos_x;
	o->y = pos_y;
	o->center_x = pos_x + o->scale/2;
	o->center_y = pos_y + o->scale/2;
}
