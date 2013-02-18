/*
 * BitOperations.c
 *
 *  Created on: 2013-01-25
 *      Author: Adam
 */

#import "BitOperations.h"

bool isBitSet( char num, int bit ) {
	return num & 1 << bit;
}
