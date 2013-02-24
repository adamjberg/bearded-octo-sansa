/*
 * CursorController.c
 *
 *  Created on: 2013-01-30
 *      Author: Adam
 */

#define START_SPEED (float) 0.1
#define CHANGE_IN_SPEED (float) 0.0001
//#include "altera_avalon_pio_regs.h"
#include "CursorController.h"

struct CursorController* initCursorController( struct Object* cursor, struct SwitchController* switchController, struct KeyController* keyController ) {
	struct CursorController* this = (struct CursorController*)malloc(sizeof(struct CursorController));
	//this->movingLeft = this->movingRight = this->movingUp = this->movingDown = false;
	this->cursor = cursor;
	this->cursor_x = 0;
	this->cursor_y = 0;
	this->y_speed = 0;
	this->x_speed = 0;
	setXY( cursor, this->cursor_x, this->cursor_y );
	return this;
}
char byte1,byte2,byte3;
/**
 * TODO: Once objects can receive float numbers the speed should be multiplied by
 * dt, so that the movement isn't depending on the loops per second
 */
void updateCursorController( struct CursorController* cursorController, float dt ) {


	if(alt_up_ps2_read_data_byte(ps2, &byte1) ==0) {
		if( isBitSet( byte1, 0 ) )
			leftMousePressed( cursorController );
		else
			leftMouseReleased( cursorController );

		while(alt_up_ps2_read_data_byte(ps2, &byte2) != 0) {
		}
		while(alt_up_ps2_read_data_byte(ps2, &byte3) !=0 ){

		}

		if (byte2<=8 )
			cursorController->x_speed = (int)byte2;
		else
			cursorController->x_speed = 0;
		if (byte3<=8 )
			cursorController->y_speed = (int)byte3;
		else {
			cursorController->y_speed = 0;
		}

	cursorController->cursor_x += cursorController->x_speed;
	cursorController->cursor_y -= cursorController->y_speed;

	if(cursorController->cursor_x > 310)
		cursorController->cursor_x = 310;
	if(cursorController->cursor_x <0)
		cursorController->cursor_x = 0;
	if( cursorController->cursor_y > 235)
		cursorController->cursor_y = 235;
	if( cursorController->cursor_y < 0)
		cursorController->cursor_y = 0;
	update((struct Cursor*)(cursorController->cursor->subObject), (int) ( cursorController->cursor_x ), (int) ( cursorController->cursor_y ) );
	}
}

void mouse_movement_listener( struct KeyController* keyController, struct CursorController* cursorController )
{
	cursorController->movingLeft = !isBitSet(keyController->keys, 3);
	cursorController->movingRight = !isBitSet(keyController->keys, 2);
	cursorController->movingDown = !isBitSet(keyController->keys, 1);
	cursorController->movingUp = !isBitSet(keyController->keys, 0);
}

void mouse_click_listener(struct SwitchController* switchController, struct CursorController* cursorController)
{
	if( isBitSet(switchController->switches, 0 ) )
		leftMousePressed( cursorController );
	else
		leftMouseReleased( cursorController );
}

void leftMouseReleased(struct CursorController* cursorController) {
	((struct Cursor*)(cursorController->cursor->subObject))->hit = 0;
}

void leftMousePressed(struct CursorController* cursorController) {
	if( ((struct Cursor*)(cursorController->cursor->subObject))->hit == 1 )
		return;
	playSoundById(audioController, LASER_SOUND, 0);
	((struct Cursor*)(cursorController->cursor->subObject))->hit = 1;
}
