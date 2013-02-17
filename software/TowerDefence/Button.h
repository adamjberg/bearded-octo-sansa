/*
 * Button.h
 *
 *  Created on: 2013-01-30
 *      Author: Danny
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include "Game.h"

struct Range;
struct Button {
	struct Object* super;
	struct SpaceShip* obj;
	struct animation* animatePic;
	struct Box3D* obj1;
	int type, red, green, blue;
	struct Range* r;
	char* name;
	bool redrawing;
	struct Env* env;
	void (*redrawImg)(struct Button*, struct Cursor*);
};

struct Button* initButton(int, int, int, struct animation*, struct animation*, struct Env*);
void killButton(struct Button*);
void setBuyButton(struct Button*, int, struct animation**);
void setTxtButton(struct Button*, int, int, int, char*, int, int, int);
void drawBuyButton(struct Button*, struct Cursor*);
void drawTxtButton(struct Button*, struct Cursor*);
void buyButtonCollide(void*, void*, struct Collidable*);
void pageButtonCollide(void*, void*, struct Collidable*);
void upgradeButtonCollide(void*, void*, struct Collidable*);
void endGameButtonCollide(void*, void*, struct Collidable*);
void startGameButtonCollide(void*, void*, struct Collidable*);
void createShip(struct Button*);
void dummyRedraw(struct Button*, struct Cursor*);
void updateView(struct Button*);
void changePage(struct Cursor*, int);
void updateStatsPage(struct SpaceShip*);
#endif /* BUTTON_H_ */
