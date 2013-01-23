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
#include "hardCode.h"
#include "Env.h"

struct animation {
	int* image;
	int prev_x, prev_y;
	int clean;
	struct animation *prev;
	struct animation *next;
	struct animation* first;
	struct animation* end;
};

void backgroundAnimation(alt_up_pixel_buffer_dma_dev*, int*);
int getColor(int, int, int);
int getColor555(int);

struct animation* initAnimation(int*, int);
void imageInDisk(struct animation*);
void addImage(struct animation*, struct animation*);
void draw(alt_up_pixel_buffer_dma_dev*, int, int, int*, int);
void cleanDrawing(alt_up_pixel_buffer_dma_dev*, int, int, int);
void animated(alt_up_pixel_buffer_dma_dev*, struct animation**, int, int, int, int);
void killAnimation(struct animation*, int);

#endif /* ANIMATION_H_ */
