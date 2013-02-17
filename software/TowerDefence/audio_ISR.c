#include "audio_ISR.h"

/***************************************************************************************
 * Audio - Interrupt Service Routine
 *
 * This interrupt service routine records or plays back audio, depending on which type
 * interrupt (read or write) is pending in the audio device.
****************************************************************************************/
void audio_ISR(struct AudioController* audioController, unsigned int id)
{
	int alt_up_audio_write_fifo(alt_up_audio_dev *audio, unsigned int *buf, int len, int channel);

	if (alt_up_audio_write_interrupt_pending(audioController->audio))	// check for write interrupt
	{
		int num_to_write = alt_up_audio_write_fifo_space( audioController->audio, ALT_UP_AUDIO_LEFT );

		alt_up_audio_write_fifo( audioController->audio, &(audioController->buffer[audioController->buf_index_play]), num_to_write, ALT_UP_AUDIO_LEFT );
		alt_up_audio_write_fifo( audioController->audio, &(audioController->buffer[audioController->buf_index_play]), num_to_write, ALT_UP_AUDIO_RIGHT );

		updateAudioController(audioController, num_to_write);
	}
	return;
}
