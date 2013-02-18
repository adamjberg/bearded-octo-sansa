/*
 * CursorController.h
 *
 *  Created on: 2013-01-30
 *      Author: Adam
 */

#ifndef CURSORCONTROLLER_H_
#define CURSORCONTROLLER_H_

#include "Game.h"
extern char byte1;
struct CursorController {
	volatile struct Object* cursor;
	bool movingLeft, movingRight, movingUp, movingDown;
	float cursor_x, cursor_y;
	float x_speed, y_speed;
};

void mouse_movement_listener(struct KeyController*, struct CursorController*);
void mouse_click_listener(struct SwitchController*,struct CursorController*);
struct CursorController* initCursorController(struct Object*, struct SwitchController*, struct KeyController*);
void updateCursorController(struct CursorController*, float);
int isLeftMouseDown();
void mouseClickDetected(struct CursorController*);
void leftMousePressed(struct CursorController*);
void leftMouseReleased(struct CursorController*);

#endif /* CURSORCONTROLLER_H_ */
