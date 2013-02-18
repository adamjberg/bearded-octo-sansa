/*
 * QuardTree.h
 *
 *  Created on: 2013-01-28
 *      Author: Danny
 */

#ifndef QUARDTREE_H_
#define QUARDTREE_H_
#include "Game.h"

struct Range {
	int x, y, width, height;
};
struct QuardTree {
	struct Object* obj;
	struct Range* r;
	int capacity;
	struct QuardTree* NWQT;
	struct QuardTree* NEQT;
	struct QuardTree* SWQT;
	struct QuardTree* SEQT;
};

struct QuardTree* initQuardTree(struct Range*);
struct Range* initRange(int, int, int, int);
bool insertObj(struct QuardTree*, struct Object*);
bool query(struct QuardTree*, struct Range*, struct Object*, struct Collidable*);
bool inBoundary(struct Range*, struct Object*);
bool isContain(struct Range*, struct Object*);
bool isCollide(struct Range*, struct Range*);
void clearTree(struct QuardTree*);
#endif /* QUARDTREE_H_ */
