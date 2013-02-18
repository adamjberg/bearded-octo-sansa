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
struct Object* initObject(int pos_x, int pos_y, int size, struct animation* s1, void* subClass) {
	struct Object* this;
	//safefree(&this);
	this = (struct Object*)malloc(sizeof(struct Object));
	if(this ==NULL)
		printf("NULL in Object!!!\n");
	this->x = pos_x;
	this->y = pos_y;
	this->class = 0;
	this->isBound = 0;
	this->prev_x = pos_x;
	this->prev_y = pos_y;
	this->center_x = pos_x + size/2;
	this->center_y = pos_y + size/2;
	this->scale = size;
	this->collideRange = size/2;
	this->curr_frame = 0;
	this->end_frame = -5;
	this->subObject = subClass;
	this->startAnimated = 1;
	this->isShowing = 1;
	this->currImg = s1;
	this->stats[0] = s1;
	if(s1 != NULL) {
		this->total_stats = 1;
	} else
		this->total_stats = 0;
	this->isSelected = 0;
	this->isRunable = 0;
	this->animate = animated;
	this->collide = dummyCollide;
	this->action = dummyAction;
	//this->r = initRange(pos_x, pos_y, size, size);

	return this;
}
/*
void setCollideRange(struct Object* this, int range) {
	this->r->x = this->center_x - range;
	this->r->y = this->center_y - range;
	this->r->width = this->r->height = range*2;
	this->collideRange = range;
}*/
/*
 * destructor of object
 */
void killObject(struct Object* this) {
	if(this == NULL) return;
	this->currImg = NULL;
	this->animate = NULL;
	this->stats[0] = NULL;
	this->stats[1] = NULL;
	this->stats[2] = NULL;
	this->collide = NULL;
	this->action = NULL;
	//alt_up_pixel_buffer_dma_draw_box(pixel_buffer,this->x, this->y, this->x+this->scale, this->y+this->scale, 0x0000, 0);
	safefree(&this);
	//free(this->r); this->r = NULL;
	//free(this); this = NULL;
	//free(this);
	//this = NULL;
}

/*
 * stats in an object define the animations that it is allow to run
 * this function will change the animation of an object
 */
void setStats(struct Object* this, int stat) {
	this->currImg = this->stats[stat];
}
void addStats(struct Object* this, struct animation* s) {
	if(s != NULL) {
		this->stats[this->total_stats] = s;
		this->total_stats++;
		this->currImg = this->stats[0];
	}
}

/*
 * change the coordinate system of an object
 */
void setXY(struct Object* o, int pos_x, int pos_y) {
	int boundx, boundy;
	o->prev_x = o->x;
	o->prev_y = o->y;

	if(o->class == cursor) {
		boundx = 310;
	} else
		boundx = 250;
	boundy = 240;
	/*if(inBoundary(initRange(0, 0, 240, 320), o)) {
		o->isBound = 1;
	}*/
	if(pos_x > boundx) {
		o->x = boundx-o->scale;
	}else if(pos_x < 5)
		o->x = 5;
	else
		o->x = pos_x;
	if(pos_y > boundy)
		o->y = boundy-o->scale;
	else if(pos_y < 5)
		o->y = 5;
	else
		o->y = pos_y;

	o->center_x = o->x + o->scale/2;
	o->center_y = o->y + o->scale/2;

}

void setXY_noBound(struct Object* o, int x, int y) {
	o->prev_x = o->x;
	o->prev_y = o->y;
	o->x = x;
	o->y = y;

	o->center_x = o->x + o->scale/2;
	o->center_y = o->y + o->scale/2;
}
void dummyCollide(void* this, void* obj, struct Collidable* c) {
	//printf("%d collides %d\n", ((struct Object*)this)->class, ((struct Object*)obj)->class);
}

void dummyAction(void* this) {
	//printf("action is done by class %d\n", ((struct Object*)this)->class);
}
