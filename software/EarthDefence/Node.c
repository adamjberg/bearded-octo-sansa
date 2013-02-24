/*
 * Node.c
 *
 *  Created on: 2013-02-07
 *      Author: Danny
 */

#include "Node.h"

struct Node2D* initNode2D(float x3d, float y3d, float z3d, int index, int offset) {
	struct Node2D* n = (struct Node2D*)malloc(sizeof(struct Node2D));
	n->x3d = x3d;
	n->y3d = y3d;
	n->z3d = z3d;
	n->x = n->prev_x = cameraX + x3d*cameraZ/(cameraZ+z3d);
	n->y = n->prev_y = cameraY - y3d*cameraZ/(cameraZ+z3d);
	relativePosition(n, index, offset);
	return n;
}

void relativePosition(struct Node2D* this, int index, int offset) {
	switch(index) {
	case 0:
		this->relative_x = -1*offset;
		this->relative_y = offset;
		this->relative_z = offset;
		break;
	case 1:
		this->relative_x = -1*offset;
		this->relative_y = -1*offset;
		this->relative_z = offset;
		break;
	case 2:
		this->relative_x = offset;
		this->relative_y = offset;
		this->relative_z = offset;
		break;
	case 3:
		this->relative_x = offset;
		this->relative_y = -1*offset;
		this->relative_z = offset;
		break;
	case 4:
		this->relative_x = -1*offset;
		this->relative_y = offset;
		this->relative_z = -1*offset;
		break;
	case 5:
		this->relative_x = -1*offset;
		this->relative_y = -1*offset;
		this->relative_z = -1*offset;
		break;
	case 6:
		this->relative_x = offset;
		this->relative_y = offset;
		this->relative_z = -1*offset;
		break;
	case 7:
		this->relative_x = offset;
		this->relative_y = -1*offset;
		this->relative_z = -1*offset;
		break;
	default:
		break;
	}
}
void updatePos(struct Node2D* this, float rx, float ry, float rz) {
	this->relative_x = rx;
	this->relative_y = ry;
	this->relative_z = rz;
}
void updateNode(struct Node2D* this, float x3d, float y3d, float z3d) {
	this->prev_x = this->x;
	this->prev_y = this->y;
	this->x = cameraX + x3d*cameraZ/(cameraZ+z3d);
	this->y = cameraY - y3d*cameraZ/(cameraZ+z3d);
	this->x3d = x3d;
	this->y3d = y3d;
	this->z3d = z3d;
}
struct Edge* initEdge(struct Node2D* n1, struct Node2D* n2) {
	struct Edge* e = (struct Edge*)malloc(sizeof(struct Edge));
	e->n1 = n1;
	e->n2 = n2;
	return e;
}

void killEdge(struct Edge* this) {
	this->n1 = NULL;
	this->n2 = NULL;
	free(this);
	this = NULL;
}
