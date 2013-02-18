/*
 * animation.c
 *
 *  Created on: 2013-01-17
 *      Author: Danny
 */

#include "animation.h"
/*
 * Constructor of Animation; start == 1 if this animation contains the first image
 * in the sequence; c == 1 if the image in the animation comes from the disk
 */
struct animation* initAnimation(int* img, int start) {
	struct animation* a;
	while((a = (struct animation*)malloc(sizeof(struct animation)))==NULL) {printf("memory allocation with aniimation\n");}
	a->prev_x = a->prev_y = 0;
	a->image = img;
	a->prev = a;
	a->next = a;
	a->end = a;
	a->clean = 0;
	if(start > 0)
		a->first = a;
	else
		a->first = NULL;
	return a;
}

void imageInDisk(struct animation* this) {
	this->clean = 1;
}
/*
 * Destructor; if attribute clean == 1, free the image
 */
void killAnimation(struct animation* this) {
	if(this->prev != NULL) {
		this->prev = NULL;
	}
	if(this->clean == 1) {
		free(this->image);
		this->image = NULL;
	}
	if(this->end != this)
		killAnimation(this->next);

	this->next = NULL;
	this->first = NULL;
	this->end = NULL;
	free(this);
	this = NULL;
}
/*
 * add an image to the animation
 */
void addImage(struct animation *curr, struct animation* n) {
	n->prev = curr->end;
	curr->end->next = n;
	curr->end = n;
	curr->first->prev = n;
	n->first = curr->first;
	n->next = curr->first;
}

void draw(int pos_x, int pos_y, int* img, int size) {
	int i, j;
	if(pos_x < 0 || pos_y < 0 || pos_x >= 320 || pos_y >= 240) return;
	for(i = 0; i < size; i++) {
	    for(j = 0; j < size; j++) {
	    	if(*(img + j*size+i) != 0) {
	    		 IOWR_16DIRECT(pixel_buffer->buffer_start_address, ((pos_y+j)*320+pos_x+i)*2, *(img + j*size+i));
	    	}
	    }
	}
}

void drawRec(int pos_x, int pos_y, int* img, int h, int w) {
	int i, j;
	if(pos_x < 0 || pos_y < 0 || pos_x >= 320 || pos_y >= 240) return;
	for(i = 0; i < w; i++) {
		for(j = 0; j < h; j++) {
		    if(*(img + j*w+i) != 0) {
		    	IOWR_16DIRECT(pixel_buffer->buffer_start_address, ((pos_y+j)*320+pos_x+i)*2, *(img + j*w+i));
		    }
		   }
	}
}
void drawBox(int x1, int y1, int x2, int y2, int color, struct Cursor* mouse) {
	if(x1 < 0 || y1 < 0) return;
	int i, j, x, y, dx, dy, w = x2-x1, h = y2-y1;
		for(i = 0; i < w; i++) {
		    for(j = 0; j < h; j++) {
		    	x = x1+i; y = y1+j;
		    	if(mouse != NULL) {
		    		dx = x-mouse->super->x; dy = y-mouse->super->y;
		    		if(dx >= 0 && dx < 10 && dy >= 0 && dy < 10)
		    			*(mouse->overlapImg+dy*10 + dx) = color;
		    	}
		    	IOWR_16DIRECT(pixel_buffer->buffer_start_address, ((y)*320+x)*2, color);
		}
	}
}
void draw_nontransparent(int pos_x, int pos_y, int* img, int size, struct Cursor* mouse) {
	int i, j, x, y, dx, dy;
	if(pos_x < 0 || pos_y < 0) return;
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			x = pos_x+i; y = pos_y+j;
			if(mouse != NULL) {
				dx = x-mouse->super->x; dy = y-mouse->super->y;
				if(dx >= 0 && dx < 20 && dy >= 0 && dy < 20)
					*(mouse->dragOverlapImg+dy*20 + dx) = *(img + j*size+i);
			}
		    if(swap == 0)
		    	IOWR_16DIRECT(pixel_buffer->buffer_start_address, (y*320+x)*2, *(img + j*size+i));
		    else
		    	IOWR_16DIRECT(pixel_buffer->back_buffer_start_address, (y*320+x)*2, *(img + j*size+i));
		}
	}
}
void cleanDrawing(int pos_x, int pos_y, int* img, int size, struct Cursor* mouse) {
	int i, j, dx, dy, x, y, buffer_size = 10;
	if(pos_x < 0 || pos_y < 0 || pos_x >= 320 || pos_y >= 240) return;
	int* buffer_img = mouse->overlapImg;
	if(mouse->buttonPushed == 1) {
		buffer_size = 20;
		buffer_img = mouse->dragOverlapImg;

	}
	for(i = 0; i < size; i++) {
	    for(j = 0; j < size; j++) {
	    	if(*(img + j*size+i) != 0) {
	    		x = pos_x+i; y = pos_y+j;
	    		if(mouse != NULL) {
					dx = x-mouse->super->x; dy = y-mouse->super->y;
					if(dx >= 0 && dx < buffer_size && dy >= 0 && dy < buffer_size)
						*(buffer_img+dy*buffer_size + dx) = 0;
	    		}
	    		IOWR_16DIRECT(pixel_buffer->buffer_start_address, ((y)*320+x)*2, 0x0000);
	    	}
	    }
	}
}

/*
 * animate the animation.  Assume the image is size x size matrix.
 * Draw the next image in next round if isForward == 1, otherwise draw the previous image in next round.
 */
void animated(struct animation** curr, int x, int y, int size, int isForward, struct Object* obj, struct Cursor* m) {
	//if(x < 0 || y < 0 || x > 315 || y > 315) return;

	if(*curr != NULL && obj->isShowing == 1) {
		if(obj->startAnimated == 1) {
			obj->startAnimated = 0;
			obj->animate_prev_x = obj->x;
			obj->animate_prev_y = obj->y;
		}
		if(isForward > 0) {
			cleanDrawing(obj->animate_prev_x, obj->animate_prev_y, (*curr)->prev->image, size, m);
		} else {
			cleanDrawing(obj->animate_prev_x, obj->animate_prev_y, (*curr)->next->image, size, m);
		}

		obj->animate_prev_x = x;
		obj->animate_prev_y = y;
		draw(x,y,(*curr)->image,size);
		if(isForward > 0) {
			*curr = (*curr)->next;
		} else {
			*curr = (*curr)->prev;
		}
	}
}

/*
 * animation for background, non-interacting objects
 */
void backgroundAnimation(int* coord) {
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[0], coord[1], 220, 210, 0x0000,swap);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[1], coord[0], 220, 210, 0x0000, swap);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[0]+20, coord[0]+10, 220, 210, 0x0000,swap);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[1]+10, coord[1]+10, 220, 210, 0x0000,swap);
	coord[0]++;
	coord[1]--;
	if(coord[0] > 200) coord[0] = 150;
	if(coord[1] < 150) coord[1] = 200;
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[0], coord[1], 220, 210, 0x3333,swap);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[1], coord[0], 220, 210, 0xFFFF, swap);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[0]+20, coord[0]+10, 220, 210, 0x1111,swap);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, coord[1]+10, coord[1]+10, 220, 210, 0xAAAA,swap);

}

/*helper function to convert 32 bit color code to 16 bit color*/

int getColor(int red, int green, int blue) {
	return (int)(((red>>3)<<11) + ((green>>2)<<5) + (blue>>3));
	//return red*2048+green*32+blue;
}
int getColor555(int color555) {
	int color = color555&0x7FFF;
	return (color&0x7C00)*2+(color&0x03E0)*2+(color&0x1F);
}
