/*
 * Env.c
 *
 *  Created on: 2013-01-19
 *      Author: Danny
 */
#include "Env.h"
/*
 The attribute o stores all objects that exist in the environment.
 The initial size is 30, which may require more space later.
 */
struct Env* initEnv() {
	struct Env* env = (struct Env*)malloc(sizeof(struct Env));
	env->o = (struct Object**)malloc(30*sizeof(struct Object*));
	env->size = 0;
	env->coord[0] = env->coord[1] = 150;
	return env;
}

void addToEnv(struct Env* e, struct Object* obj) {
	e->o[e->size] = obj;
	e->size++;
}

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
