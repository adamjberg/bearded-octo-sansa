/*
 * Box3D.c
 *
 *  Created on: 2013-02-07
 *      Author: Danny
 */
#include "Box3D.h"
struct Box3D* initBox3D(int x, int y, int z, int size, int color) {
	struct Box3D* box = (struct Box3D*)malloc(sizeof(struct Box3D));
	box->x = x; box->y = y; box->z = z; box->scale = size;
	int offset = size/2;
	float cord_x = x - cameraX;
	float cord_y = cameraY - y;
	box->nodes[0] = initNode2D(cord_x - offset, cord_y + offset, z+offset, 0, offset);
	box->nodes[1] = initNode2D(cord_x - offset, cord_y - offset, z+offset, 1, offset);
	box->nodes[2] = initNode2D(cord_x + offset, cord_y + offset, z+offset, 2, offset);
	box->nodes[3] = initNode2D(cord_x + offset, cord_y - offset, z+offset, 3, offset);
	box->nodes[4] = initNode2D(cord_x - offset, cord_y + offset, z-offset, 4, offset);
	box->nodes[5] = initNode2D(cord_x - offset, cord_y - offset, z-offset, 5, offset);
	box->nodes[6] = initNode2D(cord_x + offset, cord_y + offset, z-offset, 6, offset);
	box->nodes[7] = initNode2D(cord_x + offset, cord_y - offset, z-offset, 7, offset);
	box->edges[0] = initEdge(box->nodes[0], box->nodes[1]);
	box->edges[1] = initEdge(box->nodes[0], box->nodes[2]);
	box->edges[2] = initEdge(box->nodes[2], box->nodes[3]);
	box->edges[3] = initEdge(box->nodes[1], box->nodes[3]);
	box->edges[4] = initEdge(box->nodes[4], box->nodes[5]);
	box->edges[5] = initEdge(box->nodes[4], box->nodes[6]);
	box->edges[6] = initEdge(box->nodes[5], box->nodes[7]);
	box->edges[7] = initEdge(box->nodes[6], box->nodes[7]);
	box->edges[8] = initEdge(box->nodes[0], box->nodes[4]);
	box->edges[9] = initEdge(box->nodes[1], box->nodes[5]);
	box->edges[10] = initEdge(box->nodes[2], box->nodes[6]);
	box->edges[11] = initEdge(box->nodes[3], box->nodes[7]);
	box->startAnimate = false;
	box->endAnimate = false;
	box->acc_y = 0.01862;
	box->acc_z = box->acc_x = 0.02;
	box->vx =box->vz = 0;
	 box->vy = -1;
	return box;
}

void drawBox3D(struct Box3D* this, int background, int color) {
	int i;
	for(i = 0; i < 12; i ++) {
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (int)this->edges[i]->n1->prev_x, (int)this->edges[i]->n1->prev_y, this->edges[i]->n2->prev_x, this->edges[i]->n2->prev_y, 0,background);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (int)this->edges[i]->n1->x, (int)this->edges[i]->n1->y, this->edges[i]->n2->x, this->edges[i]->n2->y, color,background);
	}
}

void killBox(struct Box3D* this) {
	int i;
	for(i = 0; i < 12; i++) {
		killEdge(this->edges[i]);
	}
	for(i = 0; i < 8; i++) {
		free(this->nodes[i]);
		this->nodes[i] = NULL;
	} free(this);
	this = NULL;
}
void updateBox3D(struct Box3D* this) {
	float cord_x = this->x - cameraX;
	float cord_y = cameraY - this->y;
	int i;
	for(i = 0; i < 8; i++) {
		updateNode(this->nodes[i], cord_x + this->nodes[i]->relative_x, cord_y + this->nodes[i]->relative_y, this->z+this->nodes[i]->relative_z-320+cameraZ);
	}
}

void rotateBox3DY(struct Box3D* this) {
	int i;
	for(i = 0; i < 8; i++) {
		updatePos(this->nodes[i], this->nodes[i]->relative_x*0.9962+this->nodes[i]->relative_z*0.08716, this->nodes[i]->relative_y, this->nodes[i]->relative_z*0.9962-this->nodes[i]->relative_x*0.08716);
	} //usleep(50000);
	updateBox3D(this);
}
void rotateBox3DX(struct Box3D* this) {
	int i;
	for(i = 0; i < 8; i++) {
		updatePos(this->nodes[i], this->nodes[i]->relative_x, this->nodes[i]->relative_y*0.9962-this->nodes[i]->relative_z*0.08716, this->nodes[i]->relative_z*0.9962+this->nodes[i]->relative_y*0.08716);
	} //usleep(50000);
	updateBox3D(this);
}
void rotateBox3DZ(struct Box3D* this) {
	int i;
	for(i = 0; i < 8; i++) {
		updatePos(this->nodes[i], this->nodes[i]->relative_x*0.9962-this->nodes[i]->relative_y*0.08716, this->nodes[i]->relative_y*0.9962+this->nodes[i]->relative_x*0.08716, this->nodes[i]->relative_z);
	} //usleep(50000);
	updateBox3D(this);
}

void animateBox0(struct Box3D* this) {

	if(this->endAnimate) {
		this->startAnimate = false;
		this->vy += this->acc_y;
		this->vz += this->acc_z;
	  	this->y+=this->vy;
	  	this->z+=this->vz;
	  	if(this->z > 2000)
	  		info.start = 0;
	  	if(this->y > 260)
	  		this->vy = -1.5;
		rotateBox3DX(this);
	} else if(this->startAnimate) {
	  	rotateBox3DZ(this);
	}
}
bool animateBox1(struct Box3D* this) {
	if(this->endAnimate) {
			this->startAnimate = false;
			this->vy += this->acc_y;
			this->vz += this->acc_z;
		  	this->y+=this->vy;
		  	this->z+=this->vz;
		  	if(this->z > 1500) {
		  		return true;
		  	}
		  	if(this->y > 260)
		  		this->vy = -1.5;
			rotateBox3DX(this);
		} else if(this->startAnimate) {

	  	rotateBox3DY(this);
	}
  	return false;
}
bool animateBox2(struct Box3D* this) {
	if(this->endAnimate) {
			this->startAnimate = false;
			this->vy += this->acc_y;
			this->vz += this->acc_z;
		  	this->y+=this->vy;
		  	this->z+=this->vz;
		  	if(this->z > 1500) {
		  		return true;
		  	}
		  	if(this->y > 260)
		  		this->vy = -1.5;
			rotateBox3DX(this);
		} else if(this->startAnimate) {

	  	rotateBox3DX(this);
	} return false;
}
