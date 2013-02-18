/*
 * Cursor.h
 *
 * Cursor class is a representation of hardware controller from the software side.  It
 * is used to handle selection and de-selection for all select-able objects inside the game.
 * An selected object may be return with cursor's coordinate for other purposes.
 * It does not process algorithm to handle placement of spaceship; however, it defines how UI
 * response to user.  An example will be that user hit the key on mouse to push a button on the 
 * Panel.  Cursor then notify the panel to response and create a spaceship (not place on the environment yet).  
 * As user hit the key again, it can pass the spaceship with x and y to other class which will
 * make decision if the tower should be placed or not.  The cursor will then notify the button to de-selected.
 *
 *  Created on: 2013-01-25
 *      Author: Danny
 */

#ifndef CURSOR_H_
#define CURSOR_H_
#include "Game.h"

struct Button;
struct Cursor {
	struct Object* super;
	struct Button* pageButtons[3];
	struct Frame* pages[5];
	struct Frame* curr_page;
	struct Object* selectedObj;
	struct Object* createdObj;
	int hit;
	int buttonPushed;
	int* overlapImg;
	int* dragOverlapImg;
	int page_button_size, page_size;
	struct Env* env;
	struct Collidable* collidable;
	struct Range* r;
};

struct Cursor* initCursor(struct Env*, struct Collidable*);
void setPanelToCursor(struct Cursor*, struct Frame*);
void update(struct Cursor*, int, int);
void collideCursor(void*, void*, struct Collidable*);
void memOverlap(struct Cursor*);
void dragOverlap(struct Cursor*);
void cursorAnimate(struct animation**, int, int, int, int, struct Object*, struct Cursor*);
void updateCursor(struct Cursor*);
void addButton(struct Cursor*, struct Button*);
void checkButtonCollision(struct Cursor*);
void setFraneToCursor(struct Cursor*, struct Frame*);
void clearPanelFromCursor(struct Cursor*);
#endif /* CURSOR_H_ */
