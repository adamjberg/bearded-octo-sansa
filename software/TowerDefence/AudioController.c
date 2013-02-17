/*
 * Audio.c
 *
 *  Created on: 2013-02-06
 *      Author: Adam
 */

#include "audio_ISR.h"
#include "AudioController.h"

/**
 * Mandatory setup function in order for audio to function
 */
void av_config_setup()
{
	alt_up_av_config_dev * av_config =
	alt_up_av_config_open_dev(AUDIO_AND_VIDEO_CONFIG_0_NAME);
	while (!alt_up_av_config_read_ready(av_config));
}

/**
 * Initializes the AudioController
 */
struct AudioController* initAudioController() {
	av_config_setup();

	struct AudioController* this = (struct AudioController*)malloc(sizeof(struct AudioController));

	this->buffer_length = 0;
	this->num_sounds = 0;
	this->audio = alt_up_audio_open_dev(AUDIO_NAME);
	this->clear_required = false;
	this->start_dirty_index = 0;
	this->end_dirty_index = 0;
	this->buffer = NULL;
//	alt_irq_register(AUDIO_IRQ, this, (void*) audio_ISR);
//	alt_irq_enable(AUDIO_IRQ);
	alt_up_audio_reset_audio_core(this->audio);

	return this;
}

/**
 * Enables the audio ISR so that sounds can be played
 */
void enableAudioController(struct AudioController* this)
{
	alt_up_audio_enable_write_interrupt(this->audio);
}

/**
 * Disables the audio ISR to prevent sounds from playing
 * Call this whenever sound isn't needed
 */
void disableAudioController(struct AudioController* this)
{
	alt_up_audio_reset_audio_core(this->audio);
	alt_up_audio_disable_write_interrupt(this->audio);
}

void updateAudioController(struct AudioController* this, int num_written)
{
	int i;
	this->buf_index_play += num_written;

	if(this->buf_index_play >= this->buffer_length)
	{
		this->buf_index_play = 0;

		if( !this->clear_required )
			return;
		else
			clear_dirty_buffer(this);

		for( i = 0; i < this->num_sounds; i++ )
		{
			if(this->sounds[ i ]->playing)
			{
				addToAudioBuffer( this, this->sounds[ i ], this->sounds[ i ]->buffer_index );
			}
		}
	}
}

/**
 * Loads the sound for the specified id
 */
void loadSound( struct AudioController* this, int id )
{
	while((this->sounds[ this->num_sounds ] = loadWavSound(id))==NULL) {
		printf("load sound id %d error\n", id);
	}

	this->num_sounds++;
}

/**
 * Clears the audio buffer so that there is no sound output
 */
void clearAudioBuffer( struct AudioController* this )
{
	int i;
	for( i = 0; i < this->buffer_length; i++ )
	{
		this->buffer[ i ] = 0;
	}
	this->buf_index_play = 0;
}

/**
 * This allows multiple sounds to play at once by adding the two sound waves together
 *
 * @param buffer - array of values to add to the buffer that will be played
 * @param length - number of values to add to the buffer
 */
void addToAudioBuffer( struct AudioController* this, struct Sound* sound, int startIndex )
{
	if( !this->clear_required )
	{
		this->clear_required = true;
		this->start_dirty_index = this->buf_index_play;
	}
	int index_to_write = this->buf_index_play;
	int sound_index = startIndex;
	do
	{
		while( sound_index < sound->length )
		{
			if( index_to_write > this->buffer_length )
			{
				update_buffer_index(sound, sound_index );
				this->end_dirty_index = this->buffer_length;
				return;
			}
			if(this->buffer == NULL || sound->buffer == NULL) return;
			this->buffer[ index_to_write ] += sound->buffer[ sound_index ];

			index_to_write++;
			sound_index++;
		}
		if( sound->loops > 0 )
		{
			sound->loops--;
			sound_index = 0;
		}
	} while( sound->loops > 0 );
	stopSound(sound);
	if( this->end_dirty_index < index_to_write )
		this->end_dirty_index = index_to_write;
}

void clear_dirty_buffer(struct AudioController* this)
{
	int index_to_clear = this->start_dirty_index;
	if(this->buffer == NULL || this->background_loop->buffer == NULL) return;
	while( index_to_clear < this->end_dirty_index )
	{
		this->buffer[ index_to_clear ] = this->background_loop->buffer[ index_to_clear ];
		index_to_clear++;
	}
	this->clear_required = false;
	this->start_dirty_index = 0;
	this->end_dirty_index = 0;
}

/**
 * Frees up memory used by sound
 */
void unloadSoundById(struct AudioController* this, int id)
{
	int i, found = 0;
	int size = this->num_sounds;
	for( i = 0; i < size; i++ )
	{
		if(found == 1) {
			this->sounds[i-1] = this->sounds[i];
		} else if( this->sounds[ i ]->id == id )
		{
			found = 1;
			unloadSound(this->sounds[ i ]);
			this->num_sounds--;
		}
	}
}

void play_background_loop(struct AudioController* this, int id )
{
	int i;
	this->background_loop = getSoundById( this, id );
	this->buffer_length = this->background_loop->length;
	this->buffer = (int *)malloc(sizeof(int) * this->buffer_length);
	for( i = 0; i < this->buffer_length; i++)
	{
		this->buffer[ i ] = this->background_loop->buffer[ i ];
	}
	this->clear_required = false;
}

void stop_background_loop(struct AudioController* this )
{
	if( this->buffer != NULL)
	{
		free( this->buffer );
		this->buffer = NULL;
		this->buffer_length = 0;
	}
}

/**
 * Starts playing a sound
 * @param sound - sound struct to be played
 * @param loops - number of times this sound should be looped
 */
void playSound(struct AudioController* this, struct Sound* sound, int loops)
{
	sound->loops = loops;
	sound->buffer_index = 0;
	sound->playing = true;
	addToAudioBuffer( this, sound, 0 );
}

/**
 * Plays a sound based on the id stored in Sound.h
 *
 * Note: Sound must be loaded first
 *
 * @param id - id of the sound
 * @param loops - number of times this sound should be looped
 */
void playSoundById( struct AudioController* this, int id, int loops )
{
	int i;
	for( i = 0; i < this->num_sounds; i++ )
	{
		if( this->sounds[ i ]->id == id )
		{
			playSound( this, this->sounds[ i ], loops );
		}
	}

}

struct Sound* getSoundById(struct AudioController* this, int id)
{
	int i;
	for( i = 0; i < this->num_sounds; i++ )
	{
		if(this->sounds[i] == NULL) return NULL;
		if( this->sounds[ i ]->id == id )
		{
			return this->sounds[ i ];
		}
	}
}
