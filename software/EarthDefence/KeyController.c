/*
 * KeyController.c
 *
 *  Created on: 2013-01-25
 *      Author: Adam
 */

#include "KeyController.h"

struct KeyController* initKeyController(void)
{
	struct KeyController* this = (struct KeyController*)malloc(sizeof(struct KeyController));
	this->keys_changed = 0;
	this->keys = 0;
	this->key_listener = NULL;
	this->listener_context = NULL;

	alt_irq_register(KEYS_IRQ, this, (void*) key_ISR);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, 0);
	return this;
}

/**
 * Add a callback function for when the keys change
 * @param this - Instance of the KeyController
 * @param listener - Function to call -- accepts the KeyController and a context object
 * @param context - A pointer to pass any value/object/struct to the callback function
 * @param keys_to_listen_to - Specify which switches should trigger the interrupt ( 0x2 would be KEY(1) -- 0x3 would be KEY(1) and KEY(0) )
 */
void addKeyListener(struct KeyController* this, void (*listener)(struct KeyController*, void *), void *context, int keys_to_listen_to)
{
	if( this->key_listener != NULL )
	{
		removeKeyListener( this );
	}
	this->key_listener = listener;
	this->listener_context = context;

	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, keys_to_listen_to);
}

/**
 * Remove the currently registered callback function
 */
void removeKeyListener(struct KeyController* this)
{
	this->key_listener = NULL;
	this->listener_context = NULL;
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE,0x0);
}

/**
 * The Interrupt service routine for the keys
 * This reads the values of the keys and then calls the registered key listener.
 */
void key_ISR(struct KeyController* this, unsigned int id)
{
	this->keys = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
	this->keys_changed = IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);

	if( this->key_listener != NULL )
		( *this->key_listener )( this, this->listener_context );

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP( KEYS_BASE, 0);
}

/**
 * Returns true if the key was pressed when the last ISR was called
 */
bool wasKeyJustPressed(struct KeyController* this, int key)
{
	return isBitSet(this->keys_changed, key) && isKeyDown(key);
}

/**
 * Returns true if the key was released when the last ISR was called
 */
bool wasKeyJustReleased(struct KeyController* this, int key)
{
	return isBitSet(this->keys_changed, key) && isKeyUp(key);
}

/**
 * These two following functions allow direct access to the current value of the keys
 */

/**
 * @param key - number of the key ( 0 - 4 )
 * returns 1 if key is down
 */
bool isKeyDown(int key) {
	return !isBitSet( IORD_8DIRECT(KEYS_BASE, 0 ), key );
}

/**
 * @param key - number of the key ( 0 - 4 )
 * returns 1 if key is up
 */
bool isKeyUp(int key) {
	return isBitSet( IORD_8DIRECT(KEYS_BASE, 0 ), key );
}
