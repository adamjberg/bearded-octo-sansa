/*
 * audio_ISR.h
 *
 *  Created on: 2013-02-02
 *      Author: Adam
 */

#include "altera_up_avalon_audio.h"
#include "AudioController.h"

#ifndef AUDIO_ISR_H_
#define AUDIO_ISR_H_

void audio_ISR(struct AudioController*, unsigned int);

#endif /* AUDIO_ISR_H_ */
