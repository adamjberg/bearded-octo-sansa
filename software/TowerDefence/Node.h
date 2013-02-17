/*
 * Node.h
 *
 *  Created on: 2013-02-07
 *      Author: Danny
 */

#ifndef NODE_H_
#define NODE_H_
#include "Game.h"

struct Node2D {
	float x, y;
	float x3d, y3d, z3d;
	float relative_x, relative_y, relative_z;
	float prev_x, prev_y;
};

struct Edge {
	struct Node2D* n1;
	struct Node2D* n2;
};

struct Node2D* initNode2D(float, float, float, int, int);
void relativePosition(struct Node2D*, int, int);
void updatePos(struct Node2D*, float, float, float);
void updateNode(struct Node2D*, float, float, float);
struct Edge* initEdge(struct Node2D*, struct Node2D*);
void killEdge(struct Edge*);
#endif /* NODE_H_ */
