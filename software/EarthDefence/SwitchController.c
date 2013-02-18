/*
 * SwitchController.c
 *
 *  Created on: 2013-01-25
 *      Author: Adam
 */

#import "SwitchController.h"

struct SwitchController* initSwitchController(void)
{
	struct SwitchController* this = (struct SwitchController*)malloc(sizeof(struct SwitchController));
	this->switches = 0;
	this->switch_listener = NULL;
	this->listener_context = NULL;

	alt_irq_register(SWITCHES_IRQ, this, (void*) switch_ISR);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCHES_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWITCHES_BASE, 0);
	return this;
}

/**
 * Add a callback function for when the switches change
 * @param this - Instance of the SwitchController
 * @param listener - Function to call -- accepts the SwitchController and a context object
 * @param context - A pointer to pass any value/object/struct to the callback function
 * @param switches_to_listen_to - Specify which switches should trigger the interrupt ( 0x2 would be SW(1) -- 0x3 would be SW(1) and SW(0) )
 */
void addSwitchListener(struct SwitchController* this, void (*listener)(struct SwitchController*, void *), void *context, int switches_to_listen_to)
{
	if( this->switch_listener != NULL )
	{
		removeSwitchListener( this );
	}
	this->switch_listener = listener;
	this->listener_context = context;

	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWITCHES_BASE, switches_to_listen_to);
}

/**
 * Remove the currently registered callback function
 */
void removeSwitchListener(struct SwitchController* this)
{
	this->switch_listener = NULL;
	this->listener_context = NULL;
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWITCHES_BASE,0x0);
}

/**
 * The Interrupt service routine for the switches
 * This reads the values of the switches and then calls the registered switch listener.
 */
void switch_ISR(struct SwitchController* this, unsigned int id)
{
	this->switches = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);

	if( this->switch_listener != NULL )
		( *this->switch_listener )( this, this->listener_context );

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP( SWITCHES_BASE, 0);
}

/**
 * These two following functions allow direct access to the current value of the switches
 */

/**
 * @param switchNum - Number of the switch to check
 * returns true if the switch is off
 */
bool isSwitchOff( int switchNum ) {
	return !isBitSet( IORD_16DIRECT( SWITCHES_BASE, 0 ), switchNum );
}


/**
 * @param switchNum - Number of the switch to check
 * returns true if the switch is off
 */
bool isSwitchOn( int switchNum ) {
	return isBitSet( IORD_16DIRECT( SWITCHES_BASE, 0 ), switchNum );
}
