/*
 * Box3D.h
 *
 *  Created on: 2013-02-07
 *      Author: Danny
 */

#ifndef BOX3D_H_
#define BOX3D_H_
#include "Game.h"
#include "Node.h"
struct Box3D {
	float x, y, z, scale;
	float acc_x, acc_y, acc_z, vx, vy, vz;
	struct Node2D* nodes[8];
	struct Edge* edges[12];
	bool startAnimate, endAnimate;
	void (*animate)(struct Box3D*);
};

struct Box3D* initBox3D(int, int, int, int, int);
void drawBox3D(struct Box3D*, int, int);
void updateBox3D(struct Box3D*);
void rotateBox3DY(struct Box3D*);
void rotateBox3DX(struct Box3D*);
void rotateBox3DZ(struct Box3D*);
void killBox(struct Box3D*);
void animateBox0(struct Box3D*);
bool animateBox1(struct Box3D*);
bool animateBox2(struct Box3D*);
#endif /* BOX3D_H_ */
