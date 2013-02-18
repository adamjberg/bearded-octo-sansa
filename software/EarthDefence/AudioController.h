/*
 * Audio.h
 *
 *  Created on: 2013-02-06
 *      Author: Adam
 */

#include <stdlib.h>
#include "altera_up_avalon_audio_and_video_config.h"
#include "sys/alt_irq.h"
#include "altera_up_avalon_audio.h"
#include "Sound.h"

#define MAX_NUM_LOADED_SOUNDS 7

#ifndef AUDIOCONTROLLER_H_
#define AUDIOCONTROLLER_H_

struct AudioController {
	alt_up_audio_dev* audio;
	volatile int buf_index_play;
	int num_sounds;
	struct Sound *sounds[MAX_NUM_LOADED_SOUNDS];
	struct Sound *background_loop;
	volatile int *buffer;
	int buffer_length;
	volatile int start_dirty_index;
	volatile int end_dirty_index;
	volatile bool clear_required;
};

struct AudioController* initAudioController(void);
void enableAudioController(struct AudioController*);
void disableAudioController(struct AudioController*);
void addToAudioBuffer(struct AudioController*, struct Sound*, int);
void play_background_loop(struct AudioController*, int );
void stop_background_loop(struct AudioController*);
void clear_dirty_buffer(struct AudioController*);
void playSoundById(struct AudioController*, int, int);
void clearAudioBuffer(struct AudioController*);
void loadSound(struct AudioController*, int);
void unloadSoundById(struct AudioController*, int);
void updateAudioController(struct AudioController*, int);
struct Sound* getSoundById(struct AudioController*, int);

#endif /* AUDIOCONTROLLER_H_ */
