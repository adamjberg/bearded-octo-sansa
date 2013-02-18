/*
 * Collidable.c
 *
 *  Created on: 2013-01-27
 *      Author: Danny
 */

#include "Collidable.h"

struct Collidable* initCollidable() {
	struct Collidable* c = (struct Collideable*)malloc(sizeof(struct Collidable));
	c->objCollection = (struct Object**)malloc(sizeof(struct Object*)*350);
	c->size = 0;
	c->qt = initQuardTree(NULL);
	return c;
}

void addObjToCollide(struct Collidable* this, struct Object* obj) {
	lock = 1;
	if(obj == NULL) {
		printf("add null collidable obj\n");
		return;
	}
	obj->isRunable = 1;
	this->objCollection[this->size++] = obj;
	//printf("collidable objs: %d\n", this->size);
	lock = 0;
}


void removeFromCollide(struct Collidable* this, struct Object* obj) {
	if(obj == NULL) {
		printf("remove null collidable obj\n");
		return;
	}
	lock = 1;
	if(obj->class == ship)
		printf("ERROR!!!\n");
	int i = 0, found = 0;
	for(i = 0; i < this->size; i++) {
		if(found == 1) {
			this->objCollection[i-1] = this->objCollection[i];
		} else if(this->objCollection[i] == obj) {
			found = 1;
		}
	}
	if(found == 1) {
		this->size--;
		this->objCollection[this->size] = NULL;
	} else {
		printf("???\n");
	}
	lock = 0;
}

/*helper function to calculate the distance between two points */
float distance_(int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	return dx*dx+dy*dy;
}

int checkCollide(struct Object* obj1, struct Object* obj2) {
	if((obj1->x > obj2->x+obj2->collideRange) || (obj1->y > obj2->y+obj2->collideRange) ||
			(obj1->x+obj1->collideRange < obj2->x) ||	(obj1->y+obj1->collideRange < obj2->y))
		return 0;
	return 1;
}
/*
 * check the collision event between each objects in the environment
 */
void checkCollision(struct Collidable* this ) {
	//if(lock == 1) return;
	int i;
	int size = this->size;
	struct Range r;
	((struct QuardTree*)(this->qt))->r = initRange(0, 0, 250, 320);
	//lock = 1;
	for(i =0; i < size; i++) {
		insertObj(this->qt, this->objCollection[i]);
	}
	lock = 1;
	for(i = 0; i < size; i++) {
		r.x = this->objCollection[i]->center_x - this->objCollection[i]->collideRange;
		r.y = this->objCollection[i]->center_y - this->objCollection[i]->collideRange;
		r.height = r.width = this->objCollection[i]->collideRange*2;
		query(this->qt, &r, this->objCollection[i], this);
	}
	clearTree(this->qt);
	lock = 0;
}


// This function is used to check if an alien enters the goal boundary (the earth).
bool checkGoal(struct Collidable* c, void* o)
{
	if( ((struct Object*)o)->class == alien && !(((struct Alien*)(((struct Object*)o)->subObject))->dead)) {
			if( (((struct Object*)o)->center_x+5) > GOALX && (((struct Object*)o)->center_y+5) > GOALY ) {
				removeFromCollide(c, (struct Object*)o);
				killAlien( (void*)(struct Alien*)(((struct Object*)o)->subObject));
				return true;
		}
	}
	return false;
}
