/*
 * button.c
 *
 *  Created on: 31. okt 2016
 *      Author: Y009
 */

/** \file button.c
**	\brief Documentation for the button module.
**
**  Describes the debouncing of a button press.
**/

//***** Header Files **********************************************************
#include <driverlib.h>
#include "button.h"
#include "../MCU/timer.h"
#include "gpio.h"

//***** DEFINES ***************************************************************
#define NOTDEBOUNCED 0xFF									/**< The value chosen as the undebounced value for the button. */
#define DEBOUNCED 0											/**< The value chosen as the debounced value for the button. */
#define ON 1												/**< Logical 1 to indicate the button is pressed. */
#define OFF 0												/**< Logical 0 to indicate the button is not pressed. */
#define ACCUARCY  0xF										/**< Variable to compare how long it has been since the debounced button has been checked. */

volatile int unbouncedBTN = 0;								/**< Modified value on which it is decided whether the button has been debounced or not. */
volatile short BTN = 0;										/**< Debounced button value. */
unsigned long long int lastCounter = 0;						/**< The last time the button was debounced. */

/*********************************************************/

short 
button_getBtn()
/** Get debounced value pressed button.
**	\return BTN
**/
{
  return BTN;
}


/*********************************************************/

void 
button_debounceBtn()
/** Debounces pressed button.
**	
**  Debounces a button press based on time and how the value on the button changes in that time.
**  \n Uses the timer module and its interruptCounter to check how much time hase passed since the
**	last check on the button.
**	\n Check time is based on the \b ACCUARCY variable. Increasing or decreasing it makes this function 
**	faster or slower accordingly.
**	\n Debounces by shifting the unbouncedBTN variable left everytime it enter the conditional then replaces the least significant bit with the current value given by the pin.
**	\n This process is repeated until a desired value is reached at which point the button is debounced.
**	\attention Button's value is not changed during the debug process - only when the NOTDEBOUNCED or DEBOUNCED values have been reached.
**/
{
	timer_diTAI(timer_timerBase);
	if ((timer_getCounter() - lastCounter) >= ACCUARCY){	/* Debouncing pin 1.1 (on board button) */
		unbouncedBTN <<= 1;
		unbouncedBTN |= gpio_getPinInput(gpio_PORT_P2, gpio_PIN1)&0x01;
		if(unbouncedBTN == NOTDEBOUNCED)
			BTN = OFF;
		else if(unbouncedBTN == DEBOUNCED)
			BTN = ON;
		lastCounter = timer_getCounter();
	}
	timer_enTAI(timer_timerBase);
}

