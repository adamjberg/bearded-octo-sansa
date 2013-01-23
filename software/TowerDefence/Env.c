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
	env->o = (struct Object**)malloc(30*sizeof(struct Object*));
	env->size = 0;
	env->start = 1;
	env->coord[0] = env->coord[1] = 150;
	return env;
}
/*
 * Adding an object to the environment so the object will be shown on the screen and
 * interacting with other objects
 */
void addToEnv(struct Env* e, struct Object* obj) {
	e->o[e->size] = obj;
	e->size++;
}
/*
 * Removing an object from the environment, which will stop the interaction and animation of
 * the object and clear the memory
 */
int removeFromEnv(struct Env* env, struct Object* obj) {
	int i, found = 0;
	for(i = 0; i < env->size; i++) {

		if(found == 1) {
			env->o[i-1] = env->o[i];
		} else if(env->o[i] == obj) {
			found = 1;
		}
	} env->size--;
	killObject(obj);
}
