/*
 * Frame.c
 *
 *  Created on: 2013-01-20
 *      Author: Danny
 */

#include "Frame.h"

struct Frame* initFrame(int x1, int y1, int x2, int y2, int type) {
	struct Frame* f = (struct Frame*)malloc(sizeof(struct Frame));
	f->super = initObject(x1, y1, x2-x1, NULL);
	f->super->class = panel;
	f->ltx = x1;
	f->lty = y1;
	f->rbx = x2;
	f->rby = y2;
	f->statesLine = 0;
	drawFrame(f, type);
	return f;
}
/*
 * Draw the frame of a new board.
 * type = 1 =>score board, type = 2 => control panel, type = 3 => status board
 */
void drawFrame(struct Frame* f, int type) {
	switch(type) {
	case 1:
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, f->ltx, f->lty, f->rbx, f->rby, getColor(0x11, 0x00, 0x22), 0);
		break;
	case 2:
		//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, f->ltx, f->lty, f->rbx, f->rby, getColor(0x33, 0xFF, 0xCC), 0);
		break;
	case 3:
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, f->ltx, f->lty, f->rbx, f->rby, getColor(0x33, 0xFF, 0xCC), 0);
		break;
	default:
		break;
	}
}

/*Update the score board with current score*/
void updateScoreFrame(struct Frame* f, struct GameInfo* g) {
	char string[12], currency[14];
	sprintf(string, "Score %d", g->score);
	sprintf(currency, "$$ -> %d", g->currency);
	alt_up_char_buffer_string(char_buffer, string, 65, 2);
	alt_up_char_buffer_string(char_buffer, currency, 65, 4);
}

/*Update the status board with message; will warp words to next line*/
void updateStatusFrame(struct Frame* f, char* msg) {
	int i = 0, isPrint = 1, len1 = 0, len2 = 0;

	if(msg != NULL) {
		if(strcmp(msg, " ") == 0) {
			while(f->statesLine > 0) {
				alt_up_char_buffer_string(char_buffer, "                 ", 64, 46+f->statesLine);
				f->statesLine--;
			}
		} else {
			char* string[30];
			char string1[15], string2[15];
			while((string[i] = strsep(&msg, " "))!=NULL) {
				strcpy(string1, string[i-1]);
				if(isPrint == 0) {
					len1 = strlen(string[i-1]);
					len2 = strlen(string[i]);
					if(len1+len2 < 15) {
						strcpy(string2, string[i]);
						strncat(string1, " ", 1);
						strncat(string1, string2, len2);
						isPrint = 1;
					}
					alt_up_char_buffer_string(char_buffer, string1, 64, 46+f->statesLine);
					f->statesLine++;
				} else isPrint = 0;
				i++;
			}alt_up_char_buffer_string(char_buffer, string[i-1], 64, 46+f->statesLine);
		}
		//free(msg);
	}
}

void updateControlFrame(struct Frame* f) {

}
