/*
 * animation.h
 *
 *  Created on: 2013-01-17
 *      Author: Danny
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "Game.h"
struct Object;
struct Cursor;

struct animation {
	int* image;
	int prev_x, prev_y;
	int clean;
	struct animation *prev;
	struct animation *next;
	struct animation* first;
	struct animation* end;
};

void backgroundAnimation(int*);
int getColor(int, int, int);
int getColor555(int);

struct animation* initAnimation(int*, int);
void imageInDisk(struct animation*);
void addImage(struct animation*, struct animation*);
void draw(int, int, int*, int);
void drawBox(int, int, int, int, int, struct Cursor*);
void draw_nontransparent( int, int, int*, int, struct Cursor*);
void cleanDrawing(int, int, int*, int, struct Cursor*);
void animated(struct animation**, int, int, int, int, struct Object*, struct Cursor*);
void killAnimation(struct animation*);
void drawRec(int, int, int*, int, int);
#endif /* ANIMATION_H_ */
