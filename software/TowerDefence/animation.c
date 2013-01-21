/*
 * animation.c
 *
 *  Created on: 2013-01-17
 *      Author: Danny
 */

#include "animation.h"

struct animation* initAnimation(int* img, int start) {
	struct animation* a = (struct animation*)malloc(sizeof(struct animation));
	a->prev_x = a->prev_y = 0;
	a->image = img;
	a->prev = a;
	a->next = a;
	a->end = a;
	if(start > 0)
		a->first = a;
	else
		a->first = NULL;
	return a;
}

void killAnimation(struct animation* this, int size) {
	if(this->prev != NULL) {
		this->prev = NULL;
	}
	//free(this->image); we don't want to free the image since image will be use for other animation
	if(this->end != this)
		killAnimation(this->next, size);

	this->next = NULL;
	this->first = NULL;
	this->end = NULL;
	free(this);
}
void addImage(struct animation *curr, struct animation* n) {
	n->prev = curr->end;
	curr->end->next = n;
	curr->end = n;
	curr->first->prev = n;
	n->first = curr->first;
	n->next = curr->first;
}

void draw(alt_up_pixel_buffer_dma_dev* pixel_buffer, int pos_x, int pos_y, int* img, int size) {
	int i, j;
	for(i = 0; i < size; i++) {
	    for(j = 0; j < size; j++) {
	    	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, i+pos_x, j+pos_y, i+pos_x, j+pos_y, *(img + j*size+i), 0);
	    }
	}
}
void cleanDrawing(alt_up_pixel_buffer_dma_dev* pixel_buffer, int pos_x, int pos_y, int size) {
	int i, j;
		for(i = 0; i < size; i++) {
		    for(j = 0; j < size; j++) {
		    	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, i+pos_x, j+pos_y, i+pos_x, j+pos_y, 0x0000, 0);
		    }
		}
}
/*
 * animate the animation.  Assume the image is size x size matrix.
 * Draw the next image in next round if isForward == 1, otherwise draw the previous image in next round.
 */
void animated(alt_up_pixel_buffer_dma_dev* pixel_buffer, struct animation** curr, int x, int y, int size, int isForward) {
	if(*curr != NULL) {
		cleanDrawing(pixel_buffer, (*curr)->prev_x, (*curr)->prev_y, size);
		(*curr)->prev_x = x;
		(*curr)->prev_y = y;
		draw(pixel_buffer, x,y,(*curr)->image,size);
		if(isForward > 0) {
			*curr = (*curr)->next;
		} else {
			*curr = (*curr)->prev;
		}
	}
}

/*
 * animation for background, non-interacting images
 */
void backgroundAnimation(alt_up_pixel_buffer_dma_dev* pixel_buffer, int* coord) {
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[0], coord[1], 220, 210, 0x0000,0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[1], coord[0], 220, 210, 0x0000, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[0]+20, coord[0]+10, 220, 210, 0x0000,0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[1]+10, coord[1]+10, 220, 210, 0x0000,0);
	coord[0]++;
	coord[1]--;
	if(coord[0] > 200) coord[0] = 150;
	if(coord[1] < 150) coord[1] = 200;
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[0], coord[1], 220, 210, 0x3333,0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[1], coord[0], 220, 210, 0xFFFF, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[0]+20, coord[0]+10, 220, 210, 0x1111,0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[1]+10, coord[1]+10, 220, 210, 0xAAAA,0);

}

/*helper function to convert 32 bit color code to 16 bit color*/

int getColor(int red, int green, int blue) {
	return (int)((red*0.25)*2048 + (green*0.5)*64 + blue*0.25);
}
