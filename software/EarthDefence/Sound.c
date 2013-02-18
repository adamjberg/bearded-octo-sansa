/*
 * Sound.c
 *
 *  Created on: 2013-02-06
 *      Author: Adam
 */

#include "Sound.h"

/**
 * Defines names of sound files so they can be grabbed by id
 */
char* getSoundFilenameFromId(int id)
{
	switch(id)
	{
		case LOOP_ONE:
			return "LONE.WAV";
		case LOOP_TWO:
			return "LTWO.WAV";
		case WIN_SOUND:
			return "WIN.WAV";
		case LOSE_SOUND:
			return "LOSE.WAV";
		case TOWER_UPGRADE_SOUND:
			return "LAZR.WAV";
		case LOSE_LIFE_SOUND:
			return "LLIFE.WAV";
		case LASER_SOUND:
			return "LAZR.WAV";
	}
	printf("SOUND ID NOT DEFINED");
	return "";
}

/**
 * Helper function to read multiple bytes and return the representative int value
 */
unsigned int readInt( int file_pointer, int numBytesToRead )
{
	unsigned int ret = 0;
	int i;
	unsigned int bytes[ numBytesToRead ];
	for( i = 0; i < numBytesToRead; i++)
	{
		if((bytes[i] = alt_up_sd_card_read(file_pointer))< 0) {
			return -1;
		}

		ret |= ( ( unsigned int ) bytes[ i ] << ( 8 * i ) );
	}
	return ret;
}

/**
 * Update the buffer index for this sound.
 * Determines if the sound is complete and allows it to continue playing if
 * it should loop.
 *
 * @param num_written - The number of values written to the buffer
 */
void update_buffer_index(struct Sound* this, int currentIndex)
{
	this->buffer_index = currentIndex;
}

/**
 * Creates a Sound struct and loads the correct wav file from the SD card
 *
 * @param id - id of the sound to load
 * @param buffer - buffer to save the sound wave into
 * NOTE: If a NULL buffer is supplied the sound will be saved in the Sound struct
 */
struct Sound* loadWavSound(int id)
{
	printf("loading sound: %s\n", getSoundFilenameFromId(id));

	struct Sound* this = (struct Sound*)malloc(sizeof(struct Sound));
	this->playing = false;
	this->id = id;
	this->loops = 0;
	this->buffer_index = 0;

	int i, index = 0;
	int file_pointer = alt_up_sd_card_fopen(getSoundFilenameFromId(id), false);
	if(file_pointer < 0) {
		unloadSound(this);
		alt_up_sd_card_fclose(file_pointer); //close the file
		return NULL;
	}

	//Start reading the wav header
	while(index < BITS_PER_SAMPLE_OFFSET)
	{
		if(alt_up_sd_card_read(file_pointer) < 0) {
			unloadSound(this);
			alt_up_sd_card_fclose(file_pointer);
			return NULL;
		}
		index++;
	}

	int bytes_per_sample = readInt( file_pointer, 2 ) / BITS_PER_BYTE;
	if(bytes_per_sample < 0) {
		unloadSound(this);
		alt_up_sd_card_fclose(file_pointer);
		return NULL;
	}
	index += 2;

	while(index < DATA_LENGTH_OFFSET)
	{
		if(alt_up_sd_card_read(file_pointer) < 0) {
			unloadSound(this);
			alt_up_sd_card_fclose(file_pointer);
			return NULL;
		}
		index++;
	}

	this->length = readInt(file_pointer, 4) / bytes_per_sample;
	if(this->length == -1) {
			unloadSound(this);
			alt_up_sd_card_fclose(file_pointer);
			return NULL;
		}
	this->buffer = (int*)malloc(sizeof(int) * this->length);

	printf("length: %u\n", this->length);

	for( i = 0; i < this->length; i++ )
	{
		this->buffer[ i ] = readInt( file_pointer, bytes_per_sample );
		if(this->buffer[ i ] == -1) {
				unloadSound(this);
				alt_up_sd_card_fclose(file_pointer);
				return NULL;
			}
	}

	printf("Sound loading complete\n");

	alt_up_sd_card_fclose(file_pointer);

	return this;
}

void stopSound(struct Sound* sound)
{
	sound->playing = false;
	sound->buffer_index = 0;
}

void unloadSound( struct Sound* sound )
{
	safefree(&sound->buffer);
	safefree(&sound);
}
