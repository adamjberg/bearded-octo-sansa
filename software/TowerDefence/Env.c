/*
 * Env.c
 *
 *  Created on: 2013-01-19
 *      Author: Danny
 */
#include "Env.h"
/*
 The attribute o stores all objects that exist in the environment.
 The capacity is now set to 30, which we may require more space later.
 */
struct Env* initEnv() {
	struct Env* env = (struct Env*)malloc(sizeof(struct Env));
	env->o = (struct Object**)malloc(350*sizeof(struct Object*));
	env->size = 0;
	env->coord[0] = env->coord[1] = 150;
	env->redraw = false;
	env->m = NULL;
	return env;
}
void setCursor(struct Env* env, struct Cursor* c) {
	env->m = c;
}
/*
 * Adding an object to the environment so the object will be shown on the screen and
 * interacting with other objects
 */
void addToEnv(struct Env* e, struct Object* obj) {
	addToEnvNR(e, obj);
	obj->isRunable = 1;
	//printf("objs in env: %d\n", e->size);
}

void addToEnvNR(struct Env* e, struct Object* obj) {
	if(obj == NULL || e == NULL) return;
	e->o[e->size] = obj;
	e->size++;
	//printf("objs in env: %d\n", e->size);
}
/*
 * Removing an object from the environment, which will stop the interaction and animation of
 * the object and clear the memory
 */
void removeFromEnv(struct Env* env, struct Object* obj) {
	if(obj == NULL) {
		printf("remove null env obj\n");
		return;
	}
	int i, found = 0;
	for(i = 0; i < env->size; i++) {
		if(found == 1) {
			env->o[i-1] = env->o[i];
		} else if(env->o[i] == obj) {
			found = 1;
			env->o[i]->isRunable = 0;
		}
	}
	if(found == 1) {
		env->size--;
		env->o[env->size] = NULL;
	} else {
		printf("objs in env: %d\n", env->size);
	}
	killObject(obj);
}

void removeFromEnvOnly(struct Env* env, struct Object* this){
	if(this == NULL) {
		printf("remove null env obj\n");
		return;
	}
	int i, found = 0;
	for(i = 0; i < env->size; i++) {
		if(found == 1) {
			env->o[i-1] = env->o[i];
		} else if(env->o[i] == this) {
			found = 1;
			env->o[i]->isRunable = 0;
		}
	}
	if(found == 1) {
		env->size--;
		env->o[env->size] = NULL;
	} else {
	printf("objs in env: %d\n", env->size);}
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer,this->x, this->y, this->x+this->scale, this->y+this->scale, 0x0000, 0);
}
void addCollisionToEnv(struct Env* this, struct Collidable* col) {
	this->col = col;
}
