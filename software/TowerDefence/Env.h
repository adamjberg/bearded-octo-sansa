/*
 * Env.h
 *
 *  Created on: 2013-01-19
 *      Author: Danny
 */

#ifndef ENV_H_
#define ENV_H_

#include "Object.h"

struct Env {
	struct Object** o;
	int size, start;
	int coord[2];
};

struct Env* initEnv();
void addToEnv(struct Env*, struct Object*);
int removeFromEnv(struct Env*, struct Object*);
#endif /* ENV_H_ */
