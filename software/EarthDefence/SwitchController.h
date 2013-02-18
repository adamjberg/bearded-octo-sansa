/*
 * SwitchController.h
 *
 *  Created on: 2013-01-25
 *      Author: Adam
 */

#ifndef SWITCHCONTROLLER_H_
#define SWITCHCONTROLLER_H_

#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "stdlib.h"
#include "stdbool.h"
#include "system.h"

struct SwitchController {
	void (*switch_listener) (struct SwitchController*, void* context);
	void *listener_context;
	volatile int switches;
};

struct SwitchController* initSwitchController(void);
void addSwitchListener(struct SwitchController*, void (*listener)(struct SwitchController*, void *), void *, int);
void removeSwitchListener(struct SwitchController*);
void switch_ISR(struct SwitchController*, unsigned int id);
bool isSwitchOn(int);
bool isSwitchOff(int);

#endif /* SWITCHCONTROLLER_H_ */
