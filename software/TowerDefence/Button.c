/*
 * Button.c
 *
 *  Created on: 2013-01-30
 *      Author: Danny
 */

#include "Button.h"

struct Button* initButton(int x, int y, int size, struct animation* np, struct animation* p, struct Env* e) {
	struct Button* b = (struct Button*)malloc(sizeof(struct Button));
	b->super = initObject(x, y, size, np, (void*)b);
	addStats(b->super, p);
	b->super->isSelected = 0;
	b->r = initRange(x, y, size, size);
	b->super->class = button;
	b->super->isShowing = 0;
	b->redrawing= true;
	b->env = e;
	b->redrawImg = dummyRedraw;
	b->obj = NULL;
	return b;
}
void killButton(struct Button* this) {
	safefree(&this->obj);
	safefree(&this->r);
	this->env = NULL;
	killObject(this->super);
	free(this);
	this = NULL;
}
void dummyRedraw(struct Button*b, struct Cursor* c) {};
void setBuyButton(struct Button* this, int type, struct animation** a) {
	this->type = type;
	this->super->collide = buyButtonCollide;
	this->redrawImg = drawBuyButton;
	this->animatePic = a[type];
	createShip(this);
	this->obj->super->isShowing = 0;
}
void setTxtButton(struct Button* this, int type, int h, int w, char* name, int red, int green, int blue) {
	this->type = type;
	this->r->height = h;
	this->r->width = w;
	this->super->collide = pageButtonCollide;
	this->redrawImg = drawTxtButton;
	this->name = name;
	this->red = red;
	this->green = green;
	this->blue = blue;
}
void drawBuyButton(struct Button* b, struct Cursor* c) {
	draw_nontransparent(b->super->x, b->super->y, b->super->currImg->image, 20, c);
	b->redrawing = false;
}

void drawTxtButton(struct Button* this, struct Cursor* c) {
	alt_up_char_buffer_string(char_buffer, "     ", this->super->prev_x/4+1, this->super->prev_y/4+1);
	alt_up_char_buffer_string(char_buffer, this->name, this->super->x/4+1, this->super->y/4+1);
	drawBox(this->super->x, this->super->y, this->super->x+this->r->width, this->super->y+this->r->height, getColor(this->red, this->green, this->blue), c);
	this->redrawing = false;
}
void buyButtonCollide(void* this, void* obj, struct Collidable* c) {
	if(((struct Object*)obj)->class == cursor && ((struct Cursor*)((struct Object*)obj)->subObject)->hit == 1) {
		if(((struct Button*)((struct Object*)this)->subObject)->obj->price > info.currency) {
			updateMSG(gamePanel.elements[2], "You do not have enough money.");
			return;
		}
		((struct Cursor*)((struct Object*)obj)->subObject)->selectedObj->isSelected = 0;
		((struct Cursor*)((struct Object*)obj)->subObject)->selectedObj = ((struct Object*)this);

		((struct Cursor*)((struct Object*)obj)->subObject)->createdObj = ((struct Button*)((struct Object*)this)->subObject)->obj->super;
		createShip(((struct Button*)((struct Object*)this)->subObject));
		((struct Cursor*)((struct Object*)obj)->subObject)->buttonPushed = 1;
		((struct Object*)this)->isSelected = 1;
		setStats((struct Object*)this, 1);

		char money[50];
		sprintf(money, "A space ship is bought!! $%d is deducted!", ((struct Button*)((struct Object*)this)->subObject)->obj->price);
		writeMSG(money );
		updateScoreMoney(0, 0-((struct Button*)((struct Object*)this)->subObject)->obj->price);

		updateView(((struct Button*)((struct Object*)this)->subObject));

	}
}

void pageButtonCollide(void* this, void* obj, struct Collidable* c) {
	switch(((struct Button*)((struct Object*)this)->subObject)->type) {
	case 10: //ctrl page
		changePage(((struct Cursor*)((struct Object*)obj)->subObject), 1);
		alt_up_char_buffer_string(char_buffer, "              ", gamePanel.elements[3]->buttons[0]->super->x/4+1, gamePanel.elements[3]->buttons[0]->super->y/4+1);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 11);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 12);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 20);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 21);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 22);
		break;
	case 11: //stats page
		changePage(((struct Cursor*)((struct Object*)obj)->subObject), 3);
		alt_up_char_buffer_string(char_buffer, "              ", gamePanel.elements[3]->buttons[0]->super->x/4+1, gamePanel.elements[3]->buttons[0]->super->y/4+1);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 11);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 12);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 20);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 21);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 22);
		alt_up_char_buffer_string(char_buffer, "              ", gamePanel.elements[1]->buttons[5]->super->x/4+1, gamePanel.elements[1]->buttons[5]->super->y/4+1);
		if((((struct Cursor*)((struct Object*)obj)->subObject)->selectedObj)->class == ship) {
			updateStatsPage(((struct SpaceShip*)(((struct Cursor*)((struct Object*)obj)->subObject)->selectedObj->subObject)));
		}
		break;
	case 12: //menu page
		changePage(((struct Cursor*)((struct Object*)obj)->subObject), 4);
		alt_up_char_buffer_string(char_buffer, "              ", gamePanel.elements[3]->buttons[0]->super->x/4+1, gamePanel.elements[3]->buttons[0]->super->y/4+1);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 11);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 12);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 20);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 21);
		alt_up_char_buffer_string(char_buffer, "              ", 65, 22);
		alt_up_char_buffer_string(char_buffer, "              ", gamePanel.elements[1]->buttons[5]->super->x/4+1, gamePanel.elements[1]->buttons[5]->super->y/4+1);
		break;
	case 13:
		((struct Button*)((struct Object*)this)->subObject)->obj1->endAnimate = true;
		break;
	case 14:
		((struct Button*)((struct Object*)this)->subObject)->obj1->endAnimate = true;
		break;
	case 15:
		((struct Button*)((struct Object*)this)->subObject)->obj1->endAnimate = true;
		break;
	default:
		break;
	}
	updateView(((struct Button*)((struct Object*)this)->subObject));
}

void updateStatsPage(struct SpaceShip* obj) {
	alt_up_char_buffer_string(char_buffer, obj->name, 65, 11);
	char level[50];
	char dmg[50];
	char range[50];
	char fireRate[50];
	sprintf(level, "LEVEL: %d ",obj->level);
	alt_up_char_buffer_string(char_buffer, level, 65, 12);
	sprintf(range, "RANGE: %d ",obj->super->collideRange);
	alt_up_char_buffer_string(char_buffer, range, 65, 20);
	sprintf(dmg, "DAMAGE: %d ",obj->damage);
	alt_up_char_buffer_string(char_buffer, dmg, 65, 21);
	sprintf(fireRate, "SPEED: %d ",obj->fire_rate);
	alt_up_char_buffer_string(char_buffer, fireRate, 65, 22);

}
void upgradeButtonCollide(void* this, void* obj, struct Collidable* c) {
	if(((struct Object*)obj)->class == cursor && ((struct Cursor*)((struct Object*)obj)->subObject)->hit == 1) {
		if(((struct Cursor*)((struct Object*)obj)->subObject)->selectedObj->class == ship) {
			upgrade(((struct SpaceShip*)((struct Cursor*)((struct Object*)obj)->subObject)->selectedObj->subObject));
			updateStatsPage(((struct SpaceShip*)(((struct Cursor*)((struct Object*)obj)->subObject)->selectedObj->subObject)));
		}
	}
}

void endGameButtonCollide(void* this, void* obj, struct Collidable* c){
	if (((struct Object*)obj)->class == cursor && ((struct Cursor*)((struct Object*)obj)->subObject)->hit == 1){
		info.isWin = false;
		info.isEnd = true;
	}
}

void startGameButtonCollide(void* this, void* obj, struct Collidable* c){
	// only set true to startButton once in 200 execution
	// this will prevent from calling checkStages multiple time.
	if (((struct Object*)obj)->class == cursor && ((struct Cursor*)((struct Object*)obj)->subObject)->hit == 1
			&& ((struct Object*)this)->isSelected == 0){
		//info.stage_counter++;
		//if (info.stage_counter > 250){
			info.startButton = true;
			((struct Object*)this)->isSelected = 1;
			//printf("startButton is set to TRUE\n");
		//	info.stage_counter = 0;
		//}
	}
}

void createShip(struct Button* b) {
	b->obj = initSpaceShip(b->super->x, b->super->y, b->type, b->animatePic);
}

void updateView(struct Button* this) {
	this->env->redraw = true;
	this->redrawing = true;
}

void changePage(struct Cursor* c, int pageNum) {
	c->curr_page = gamePanel.elements[pageNum];
	gamePanel.elements[pageNum]->redrawing = true;
}
