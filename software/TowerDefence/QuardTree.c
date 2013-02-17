/*
 * QuardTree.c
 *
 *  Created on: 2013-01-28
 *      Author: Danny
 */

#include "QuardTree.h"



struct QuardTree* initQuardTree(struct Range* r) {
	struct QuardTree* qt = (struct QuardTree*)malloc(sizeof(struct QuardTree));
	if(qt == NULL) {
		printf("Quard Tree is NULL!!!\n");
		return NULL;
	}
	qt->NWQT = qt->NEQT = qt->SWQT = qt->SEQT = NULL;
	qt->capacity = 0;
	qt->r = r;
	qt->obj = NULL;
	return qt;
}

struct Range* initRange(int x, int y, int h, int w) {
	struct Range* r = (struct Range*)malloc(sizeof(struct Range));
	r->x = x; r->y = y; r->height = h; r->width = w;
	return r;
}

bool inBoundary(struct Range* r2, struct Object* obj) {
	if(obj == NULL) return false;
	if((obj->x > r2->x+r2->width) || (obj->y +obj->collideRange*2 < r2->y) ||
				(obj->x + obj->collideRange*2 < r2->x ) ||	(obj->y > r2->y + r2->height))
		return false;
	return true;
}

bool isContain(struct Range* r, struct Object* obj) {
	if(obj == NULL) return false;
	if((obj->center_x < r->x) || (obj->center_x > r->x+r->width) || (obj->center_y < r->y) || (obj->center_y > r->y+r->height)) {
		return false;
	} return true;
}
bool insertObj(struct QuardTree* this, struct Object* obj) {
	if(obj == NULL) return false;
	int h2, w2;
	if(!isContain(this->r, obj))
		return false;
	if(this->capacity == 0) {
		//printf("Obj is position at (%d,%d)\n", obj->x, obj->y);
		this->obj = obj;
		this->capacity = 1;
		return true;
	}
	if(this->NWQT == NULL) {
		h2 = this->r->height/2;
		w2 = this->r->width/2;
		//alt_up_pixel_buffer_dma_draw_line(pixel_buffer, this->r->x, this->r->y+h2 , this->r->width+this->r->x, this->r->y+h2, 0xFFFF,0);
		//alt_up_pixel_buffer_dma_draw_line(pixel_buffer, this->r->x+ w2, this->r->y, this->r->x + w2, this->r->y+this->r->height, 0xFFFF,0);
		this->NWQT = initQuardTree(initRange(this->r->x, this->r->y, h2, w2));
		//ref[indexRef++] = &this->NWQT;
		this->NEQT = initQuardTree(initRange(this->r->x+w2, this->r->y, h2, w2));
		//ref[indexRef++] = &this->NEQT;
		this->SWQT = initQuardTree(initRange(this->r->x, this->r->y+h2, h2, w2));
		//ref[indexRef++] = &this->SWQT;
		this->SEQT = initQuardTree(initRange(this->r->x+w2, this->r->y+h2, h2, w2));
		//ref[indexRef++] = &this->SEQT;
	}
	if(insertObj(this->NWQT, obj)) return true;
	if(insertObj(this->NEQT, obj)) return true;
	if(insertObj(this->SWQT, obj)) return true;
	if(insertObj(this->SEQT, obj)) return true;
	return false;
}

bool query(struct QuardTree* this, struct Range* r, struct Object* obj, struct Collidable* c) {
	if(obj == NULL) {
		return false;
	}
	if(!isCollide(this->r, r)) return true;
	if(inBoundary(r, this->obj) && this->obj != obj) {
			//printf("%d collides %d at (%d, %d) and (%d, %d)\n", this->obj->class, obj->class, this->obj->x, this->obj->y, obj->x, obj->y);
		if(obj == NULL) printf("collide obj is null\n");
		obj->collide((void*)obj, (void*)(this->obj), c);
	}
	if(this->NWQT == NULL) return true;
	if(!query(this->NWQT, r, obj, c)) return false;
	if(!query(this->NEQT, r, obj, c)) return false;
	if(!query(this->SWQT, r, obj, c)) return false;
	if(!query(this->SEQT, r, obj, c)) return false;
	return true;
}

bool isCollide(struct Range* r1, struct Range* r2) {
	if(r1 == NULL || r2 == NULL) {
		printf("isCollide got null range\n");
		return false;
	}
	if((r1->x >= r2->x+r2->width) || (r1->y >= r2->y+r2->height) ||
				(r1->x+r1->width < r2->x) || (r1->y+r1->height < r2->y))
			return false;
		return true;
}

void clearTree(struct QuardTree* this) {
	if(this == NULL) return;
	if(this->NWQT == NULL) {
		free(this->r);
		this->r = NULL;
		//safefree(&this->r);
		this->obj = NULL;
		this->capacity = 0;
		return;
	}
	clearTree(this->NWQT);
	free(this->NWQT); this->NWQT = NULL;
	clearTree(this->NEQT);
	free(this->NEQT); this->NEQT = NULL;
	clearTree(this->SWQT);
	free(this->SWQT); this->SWQT = NULL;
	clearTree(this->SEQT);
	free(this->SEQT); this->SEQT = NULL;
	this->capacity = 0;
	this->obj = NULL;
	free(this->r); this->r = NULL;
}
