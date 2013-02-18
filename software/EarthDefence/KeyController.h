/*
 * KeyController.h
 *
 *  Created on: 2013-01-25
 *      Author: Adam
 */

#ifndef KEYCONTROLLER_H_
#define KEYCONTROLLER_H_

#include "altera_avalon_pio_regs.h"
#include "stdbool.h"
#include "stdlib.h"
#include "system.h"

#include "BitOperations.h"

struct KeyController {
	void (*key_listener) (struct KeyController*, void* context);
	void *listener_context;
	volatile int keys;
	volatile int keys_changed;
};

struct KeyController* initKeyController(void);
void addKeyListener(struct KeyController*, void (*listener)(struct KeyController*, void *), void *, int);
void removeKeyListener(struct KeyController*);
void key_ISR(struct KeyController*, unsigned int);
bool wasKeyJustPressed(struct KeyController*, int);
bool wasKeyJustReleased(struct KeyController*, int);

/**
 * @param key - number of the key ( 0 - 4 )
 * returns 1 if key is down
 */
bool isKeyDown(int key);

/**
 * @param key - number of the key ( 0 - 4 )
 * returns 1 if key is up
 */
bool isKeyUp(int key);

#endif /* KEYCONTROLLER_H_ */
