/*
 * Frame.c
 *
 *  Created on: 2013-01-20
 *      Author: Danny
 */

#include "Frame.h"

struct Frame* initFrame(int x1, int y1, int x2, int y2, struct Env* e) {
	struct Frame* f = (struct Frame*)malloc(sizeof(struct Frame));
	f->super = NULL;
	f->ltx = x1;
	f->lty = y1;
	f->rbx = x2;
	f->rby = y2;
	f->prev_line = 0;
	f->statesLine = 0;
	f->elements_size = f->buttons_size = 0;
	f->total_elements_size = 0;
	f->elements = NULL;
	f->redrawing = false;
	f->env = e;
	f->buttons = NULL;
	return f;
}
struct Frame* initMainMenu(struct Env* p, struct Box3D* box, struct Box3D* box1, struct Box3D* box2, struct Cursor* mouse) {
	struct Frame* f = initFrame(0, 0, 320, 240, p);
	f->buttons = (struct Buttons**)malloc(sizeof(struct Button*)*3);
	f->buttons[0]= initButton(cameraX + (box->x-cameraX)*cameraZ/(cameraZ+box->z), cameraY - (cameraY-box->y)*cameraZ/(cameraZ+box->z), 10, NULL, NULL, p);
	f->buttons[0]->obj1 = box;
	int i;
	setTxtButton(f->buttons[0], 13, 20, 15, "Start", 0, 0, 0);
	f->buttons[1] = initButton(cameraX + (box1->x-cameraX)*cameraZ/(cameraZ+box1->z), cameraY - (cameraY-box1->y)*cameraZ/(cameraZ+box1->z), 10, NULL, NULL, p);
	f->buttons[1]->obj1 = box1;
	setTxtButton(f->buttons[1], 14, 20, 15, "Score", 0, 0, 0);
	f->buttons[2] = initButton(cameraX + (box2->x-cameraX)*cameraZ/(cameraZ+box2->z), cameraY - (cameraY-box2->y)*cameraZ/(cameraZ+box2->z), 10, NULL, NULL, p);
	f->buttons[2]->obj1 = box2;
	setTxtButton(f->buttons[2], 15, 20, 15, "Instruction", 0, 0, 0);
	f->buttons_size = 3;
	for(i = 0; i < 3; i++) {
		drawTxtButton(f->buttons[i], mouse);
	} return f;
}
void removeFrame(struct Frame* this) {
	int i;
	for(i = 0; i < this->elements_size; i++) {
		removeFrame(this->elements[i]);
	}
	for(i = 0; i < this->buttons_size; i ++) {
		killButton(this->buttons[i]);
	} free(this);
	this= NULL;
}
struct Frame* initGamePanel(int x1, int y1, int x2, int y2, struct Env* e, struct animation** a) {

	struct Frame* f = initFrame(x1, y1, x2, y2, e);
	f->redrawing = true;
	f->elements = (struct Frame**)malloc(sizeof(struct Frame*)*5);
	f->elements[0] = initScoreBoard(255, 5, 315, 28);
	f->elements[0]->redrawImg = redrawSBoard;
	f->elements[1] = initCtrlPanel(255, 40, 315, 170, e, a);
	f->elements[1]->redrawImg = redrawCPanel;
	f->elements[2] = initMsgBoard(255, 180, 315, 235);
	f->elements[2]->redrawImg = redrawMBoard;
	f->elements_size = 5;
	f->total_elements_size = 5;
	f->elements[3] = initStatsPanel(255, 40, 315, 170, e, a);
	f->elements[3]->redrawImg = redrawSPanel;
	f->elements[4] = initMenuPanel(255, 40, 315, 170, e, a);
	f->elements[4]->redrawImg = redrawMPanel;
	f->buttons = (struct Buttons**)malloc(sizeof(struct Button*)*3);
	f->buttons[0] = initButton(255, 32, 20, NULL, NULL, e);
	setTxtButton(f->buttons[0], 10, 8, 18, "Ctrl", 0x80, 0x80, 0x80);
	f->buttons[1] = initButton(273, 32, 20, NULL, NULL, e);
	setTxtButton(f->buttons[1], 11, 8, 23, "Stats", 0x48, 0x3a, 0x3a);
	f->buttons[2] = initButton(296, 32, 20, NULL, NULL, e);
	setTxtButton(f->buttons[2], 12, 8, 18, "Menu", 0xA9, 0xA9, 0xA9);
	f->buttons_size = 3;

	e->redraw = true;
	return f;
}

struct Frame* initScoreBoard(int x1, int y1, int x2, int y2) {

	struct Frame* f = initFrame(x1, y1, x2, y2, NULL);
	f->redrawing = true;
	return f;
}

struct Frame* initCtrlPanel(int x1, int y1, int x2, int y2, struct Env* e, struct animation** a) {
	int i;
	int* b0, b0p, b1, b1p, b2, b2p, b3, b3p, b4, b4p;
	loadSDImage("S125.BMP", &b0);
	loadSDImage("S10P.BMP", &b0p);
	loadSDImage("S014.BMP", &b1);
	loadSDImage("S00P.BMP", &b1p);
	loadSDImage("S20.BMP", &b2);
	loadSDImage("S20P.BMP", &b2p);
	loadSDImage("S313.BMP", &b3);
	loadSDImage("S30P.BMP", &b3p);
	loadSDImage("S40.BMP", &b4);
	loadSDImage("S40P.BMP", &b4p);

	struct Frame* f = initFrame(x1, y1, x2, y2, e);
	f->redrawing = true;
	f->buttons = (struct Button**)malloc(sizeof(struct Button*)*5);
	f->buttons[0] = initButton(x1+5, y1+20, 20, initAnimation(b1, 1), initAnimation(b1p, 1), e);
	f->buttons[1] = initButton(x1+30, y1+20, 20, initAnimation(b0, 1), initAnimation(b0p, 1), e);
	f->buttons[2] = initButton(x1+5, y1+45, 20, initAnimation(b2, 1), initAnimation(b2p, 1), e);
	f->buttons[3] = initButton(x1+30, y1+45, 20, initAnimation(b3, 1), initAnimation(b3p, 1), e);
	f->buttons[4] = initButton(x1+5, y1+70, 20, initAnimation(b4, 1), initAnimation(b4p, 1), e);
	f->buttons_size = 6;
	for(i = 0; i < f->buttons_size - 1; i++) {
		setBuyButton(f->buttons[i], i, a);
	}//setBuyButton(f->buttons[4], 3, a);
	f->buttons[5] = initButton(x1+5, y1+118, 20, NULL, NULL, e);
	setTxtButton(f->buttons[5], 21, 10, 30, "Start", 0, 0, 0);
	f->buttons[5]->super->collide = startGameButtonCollide;
	f->buttons[5]->redrawing = false;
	return f;
}

struct Frame* initStatsPanel(int x1, int y1, int x2, int y2, struct Env* e, struct animation** a) {
	int i;

	struct Frame* f = initFrame(x1, y1, x2, y2, e);
	f->redrawing = false;
	f->buttons = (struct Button**)malloc(sizeof(struct Button*)*3);
	f->buttons[0] = initButton(x1+5, y1+20, 20, NULL, NULL, e);
//	f->buttons[1] = initButton(x1+30, y1+20, 20, initAnimation(b0, 1), initAnimation(b0p, 1), e);
//	f->buttons[2] = initButton(x1+5, y1+45, 20, initAnimation(b2, 1), initAnimation(b2p, 1), e);
//	f->buttons[3] = initButton(x1+30, y1+45, 20, initAnimation(b3, 1), initAnimation(b3p, 1), e);
//	f->buttons[4] = initButton(x1+5, y1+70, 20, initAnimation(b3, 1), initAnimation(b3p, 1), e);
	f->buttons_size = 1;
		setTxtButton(f->buttons[0], 20, 10, 20, "Upgrade", 0x48, 0x3a, 0x3a);
		f->buttons[0]->super->collide = upgradeButtonCollide;
		f->buttons[0]->redrawing = false;
	return f;
}
struct Frame* initMenuPanel(int x1, int y1, int x2, int y2, struct Env* e, struct animation** a) {

	struct Frame* f = initFrame(x1, y1, x2, y2, e);
	f->redrawing = false;
	f->buttons = (struct Button**)malloc(sizeof(struct Button*));
	f->buttons[0] = initButton(x1+5, y1+20, 20, NULL, NULL, e);
	f->buttons_size = 1;
	setTxtButton(f->buttons[0], 21, 10, 40, "End Game", 0xFF, 0, 0);
	f->buttons[0]->super->collide = endGameButtonCollide;
	f->buttons[0]->redrawing = false;
	return f;
}
struct Frame* initMsgBoard(int x1, int y1, int x2, int y2) {
	struct Frame* f = initFrame(x1, y1, x2, y2, NULL);
	f->redrawing = true;
	f->str = (char*)malloc(sizeof(char)*30);
	return f;
}

/*Update the score board with current score*/
void updateScoreFrame() {
	char string[30], currency[30], invaded[30];
	sprintf(string, "Score %d ", info.score);
	sprintf(currency, "$$ -> %d ", info.currency);
	sprintf(invaded, "Aliens %d ", info.invadedAlien);
	alt_up_char_buffer_string(char_buffer, string, 65, 2);
	alt_up_char_buffer_string(char_buffer, currency, 65, 4);
	alt_up_char_buffer_string(char_buffer, invaded, 65, 6);
}

void writeMSG(char* str) {
	updateMSG(gamePanel.elements[2], str);
}
/*Update the message board with message; will warp words to next line
 * if message has already displayed, it will reprint it on the same line
 *
 * */
int strsep_(char* string, char sep, char** arr) {
	int i, j = 0, index = 0;
	int len = strlen(string);
	arr[0] = (char*)malloc(sizeof(char)*15);
	for(i = 0; i < len; i++) {
		if(string[i] == sep && i < len-1) {
			arr[index][j] = '\0';
			j = 0;
			index++;
			arr[index] = (char*)malloc(sizeof(char)*15);
		} else {
			arr[index][j] = string[i];
			j++;
		}
	}
	return index+1;
}
void updateMSG(struct Frame* f, char* msg) {

	if(f->statesLine > 10) {
		clearMBoard();
	}
	int i = 0, len = 15, len1 = 0, len2 = 0, j = 0;
	char* line = (char*)malloc(sizeof(char)*30);
	line[0] = '\0';
	char** str;
	if((str = (char**)malloc(sizeof(char*)*40))==NULL) printf("NULL!!!\n");
	if((len1 = strsep_(msg, ' ', str)) > 40) printf("MEMORY Overflow!\n");
	for(i = 0; i < len1; i++) {
		len2 = strlen(str[i]);
		if((len -= len2+1)>0 ) {
			strncat(line, str[i], len2);
			strncat(line, " ", 1);
		} else {
			for(j = 0; j <= len+len2; j++) {
				strncat(line, " ", 1);
			}
			alt_up_char_buffer_string(char_buffer, line, 64, 46+f->statesLine);
			f->statesLine++;
			len = 15 - len2 - 1;
			strncpy(line, str[i], len2);
			line[len2] = '\0';
			strncat(line, " ", 1);
		}
	}for(j = 0; j <= len; j++) {
		strncat(line, " ", 1);
	}
/*	char* line = (char*)malloc(sizeof(char)*30);
	line[0] = '\0';
	while((str[i] = strsep(&temp, " "))!=NULL) {
		len1 = strlen(str[i]);
		if((len -= len1+1) > 0) {
			strncat(line, str[i], len1);
			strncat(line, " ", 1);
		} else {
			for(j = 0; j < len+len1; j++) {
				strncat(line, " ", 1);
			}
			alt_up_char_buffer_string(char_buffer, line, 64, 46+f->statesLine);
			f->statesLine++;
			len = 15 - len1 - 1;
			strncpy(line, str[i], len1);
			line[len1] = '\0';
			strncat(line, " ", 1);
		} i++;
	}
	for(j = 0; j < len; j++) {
		strncat(line, " ", 1);
	}*/
	alt_up_char_buffer_string(char_buffer, line, 64, 46+f->statesLine++);
	alt_up_char_buffer_string(char_buffer, "               ", 64, 46+f->statesLine++);
	for(j = 0; j < len1; j++) {
		free(str[j]);
		str[j] = NULL;
	}

	safefree(&str);
	safefree(&line);
	//safefree(&temp);
//	free(msg);
//	msg = NULL;
}

void clearMBoard() {
	int i;
	for(i = 0; i < 14; i++) {
		alt_up_char_buffer_string(char_buffer, "               ", 64, 46+i);
	} gamePanel.elements[2]->statesLine = 0;
}
void redraw(struct Cursor* c) {
	int i, j;
	if(gamePanel.redrawing){
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 250, 0, 320, 240, 0x7BEF, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 253, 3, 317, 237, 0xBDF7, 0);
		redraw_(&gamePanel, c);
		gamePanel.redrawing = false;
	}
	else {
		for(i = 0; i < gamePanel.buttons_size; i++) {
			if(gamePanel.buttons[i]->redrawing) {
				gamePanel.buttons[i]->redrawImg(gamePanel.buttons[i], c);
				gamePanel.buttons[i]->redrawing = false;
			}
		}
		for(i = 0; i <gamePanel.elements_size; i++) {
			if(gamePanel.elements[i]->redrawing) {
				gamePanel.elements[i]->redrawImg(gamePanel.elements[i], c);
				redraw_(gamePanel.elements[i], c);
			} else {
				for(j = 0; j < gamePanel.elements[i]->buttons_size; j++) {
					if(gamePanel.elements[i]->buttons[j]->redrawing) {
						gamePanel.elements[i]->buttons[j]->redrawImg(gamePanel.elements[i]->buttons[j], c);
					}
				}
			}
		}
	}
}

void redraw_(struct Frame* f, struct Cursor* c) {
	int i;
	for(i = 0; i < f->elements_size; i++) {
		if(f->elements[i]->redrawing) {
			f->elements[i]->redrawImg(f->elements[i], c);
			redraw_(f->elements[i], c);
			f->elements[i]->redrawing = false;
		}
	}
	for(i = 0; i < f->buttons_size; i++) {
		f->buttons[i]->redrawImg(f->buttons[i], c);
		f->buttons[i]->redrawing = false;
	}
}
void redrawSBoard(struct Frame* f, struct Cursor* c) {
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, f->ltx, f->lty, f->rbx, f->rby, getColor(0x77, 0xb5, 0xfe), 0);
	f->redrawing = false;
}

void redrawCPanel(struct Frame* f, struct Cursor* c) {
	drawBox(f->ltx, f->lty, f->rbx, f->rby, getColor(0x80, 0x80, 0x80), c);
	f->redrawing = false;
}
void redrawSPanel(struct Frame* f, struct Cursor* c) {
	drawBox(f->ltx, f->lty, f->rbx, f->rby, getColor(0x48, 0x3a, 0x3a), c);
	f->redrawing = false;
}
void redrawMPanel(struct Frame* f, struct Cursor* c) {
	drawBox(f->ltx, f->lty, f->rbx, f->rby, getColor(0xA9, 0xA9, 0xA9), c);
	f->redrawing = false;
}
void redrawMBoard(struct Frame* f, struct Cursor* c) {
	drawBox(f->ltx, f->lty, f->rbx, f->rby, getColor(0x48, 0x3a, 0x3a), c);
	f->redrawing = false;
}
