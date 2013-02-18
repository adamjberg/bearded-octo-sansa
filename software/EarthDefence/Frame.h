/*
 * Frame.h
 *
 *  Created on: 2013-01-20
 *      Author: Danny
 */

#ifndef FRAME_H_
#define FRAME_H_
#include "Game.h"
#include "GameInfo.h"

struct Cursor;
struct Collidable;
struct Box3D;
struct Frame {
	int ltx, lty, rbx, rby;
	int statesLine, prev_line;
	int elements_size, buttons_size;
	int total_elements_size;
	struct Button** buttons;
	char* str;
	bool redrawing;
	struct Object* super;
	struct Frame** elements;
	struct Env* env;
	void (*redrawImg)(struct Frame*, struct Cursor*);
};

struct Frame* initFrame(int, int, int, int, struct Env*);
void removeFrame(struct Frame*);
struct Frame* initMainMenu(struct Env*, struct Box3D*, struct Box3D*, struct Box3D*, struct Cursor*);
struct Frame* initGamePanel(int, int, int, int, struct Env*, struct animation**);
struct Frame* initScoreBoard(int, int, int, int);
struct Frame* initCtrlPanel(int, int, int, int, struct Env*, struct animation**);
struct Frame* initMsgBoard(int, int, int, int);
struct Frame* initStatsPanel(int, int, int, int, struct Env*, struct animation**);
struct Frame* initMenuPanel(int, int, int, int, struct Env*, struct animation**);
void updateScoreFrame();
void updateMSG(struct Frame*, char*);
void updateScoreBoard( struct Frame*);
void updateMsgBoard(struct Frame*);
void writeMSG(char*);
void redraw( struct Cursor*);
void redraw_(struct Frame*, struct Cursor*);
void redrawSBoard(struct Frame*, struct Cursor*);
void redrawCPanel(struct Frame*, struct Cursor*);
void redrawSPanel(struct Frame*, struct Cursor*);
void redrawMPanel(struct Frame*, struct Cursor*);
void redrawMBoard(struct Frame*, struct Cursor*);
void clearMBoard();
#endif /* FRAME_H_ */
