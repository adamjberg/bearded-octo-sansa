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
	int size;
	int coord[2];
	struct Collidable* col;
	struct Cursor* m;
	bool redraw;
};

struct Env* initEnv();
void addCollisionToEnv(struct Env*, struct Collidable*);
void addToEnv(struct Env*, struct Object*);
void addToEnvNR(struct Env*, struct Object*);
void removeFromEnv(struct Env*, struct Object*);
void removeFromEnvOnly(struct Env*, struct Object*);
void setCursor(struct Env*, struct Cursor*);
#endif /* ENV_H_ */
