/*
 * Sound.h
 *
 *  Created on: 2013-02-06
 *      Author: adam_000
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "altera_up_sd_card_avalon_interface.h"
#include "AudioController.h"
#include "Game.h"

#define LOOP_ONE 1
#define LOOP_TWO 2
#define WIN_SOUND 3
#define LOSE_SOUND 4
#define TOWER_UPGRADE_SOUND 5
#define LOSE_LIFE_SOUND 6
#define LASER_SOUND 7

#define BITS_PER_BYTE 8
#define BITS_PER_SAMPLE_OFFSET 34
#define DATA_LENGTH_OFFSET 40

struct Sound {
	unsigned int id;
	unsigned int buffer_index;
	unsigned int length;
	volatile unsigned int loops;
	int *buffer;
	volatile bool playing;
};

void update_buffer_index(struct Sound*, int);
struct Sound* loadWavSound(int);
void unloadSound(struct Sound*);
void stopSound(struct Sound*);

#endif /* SOUND_H_ */
