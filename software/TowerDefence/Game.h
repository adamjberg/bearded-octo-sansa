/*
 * Game.h
 *
 *  Created on: 2013-01-20
 *      Author: Danny
 */

#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "sys/alt_alarm.h"
#include "animation.h"
#include "Object.h"
#include <altera_up_sd_card_avalon_interface.h>
#include "hardCode.h"
#include "Env.h"
#include "Frame.h"

alt_up_pixel_buffer_dma_dev* pixel_buffer;
alt_up_char_buffer_dev* char_buffer;
extern int swap;
#endif /* GAME_H_ */
