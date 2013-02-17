/*
 * Interface.c
 *
 *  Created on: 2013-02-12
 *      Author: Danny
 */

#include "Interface.h"
#include "stdlib.h"
#include "stdio.h"
#include <io.h>
void waveLEDR() {
	int i;
	int k = 1;
	for(i = 0; i < 100; i++) {
		IOWR_8DIRECT(LEDR_BASE, 0, k);
		k = k*2;
		usleep(20000);
		if(k > 65536) k = 1;
	}
}
