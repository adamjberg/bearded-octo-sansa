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
	env->coord[0] = env->coord[1] = 0;
	return env;
}
void addToEnv(struct Env* e, struct Object* obj) {
	e->o[e->size] = obj;
	e->size++;
}
