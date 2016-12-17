/*
 * ultraS.c
 *
 *  Created on: 7. nov 2016
 *      Author: Y009
 */

/** \file ultraS.c
**	\brief Documentation for the ultrasonic module.
**
**  Describes the workings of the ultrasonic module.
**/

//***** Header Files **********************************************************
#include <driverlib.h>
#include "ultraS.h"
#include "../MCU/counter.h"
#include "../MCU/timer.h"
#include "stdio.h"
#include "gpio.h"

//***** DEFINES ***************************************************************

#define MAXDIST 23200							/**< 23200us is 4m. */
#define MINDIST 116								/**< 116us is 2cm. */
#define CMCONST 58								/**< Constant to divide time with to get distance in centimeters. */
#define CYCLE 0xFDE8							/**< Timer size - added to endTime when rollover happens.*/

void resetTimes();

unsigned volatile long long int startTime;              /**< Time captured on usmodule's rising edge echo signal.*/
unsigned volatile long long int endTime;                /**< Time captured on usmodule's falling edge echo signal. */
unsigned volatile long long int startTimeMult;          /**< Insurance variable to insure that startTime and endTime are correct. */
unsigned volatile long long int endTimeMult;            /**< Insurance variable to insure that startTime and endTime are correct. */
unsigned volatile long int distance;                    /**< Distance calculated based on start and end time and it's multipliers */

enum status usStatus = US_IDLE;                         /**< Set finite state machine to idle/waiting state. */
enum dataStatus usDataStatus = US_DATA_FALSE;           /**< Set finite state machine to start in false state to insure no false information. */

/*********************************************************/
void 
ultraS_init()
/** Set ultrasonic module to be ready for work.
**
**  Set's appropriate FSM states and resets start and end times to 0.
**/
{
    usStatus = US_WORKING;
    resetTimes();
    usStatus = US_IDLE;
}

/*********************************************************/
void 
ultraS_sendSignal()
/** Send signal to the physical ultrasonic module.
**
**  Set's appropriate FSM state. Send's a high signal for required time - 10uS.
**  \n Delays system to not send multiple signals at the same time.
**/
{
    usStatus = US_WORKING;
	gpio_setPinHigh (gpio_PORT_P1, gpio_PIN4);
	__delay_cycles(160U);												/* trigger time */
	gpio_setPinLow (gpio_PORT_P1, gpio_PIN4);
	__delay_cycles(96000U);												/* echo wait period */
}

/*********************************************************/
void 
ultraS_prepInfo()
/** Calculates measured distance.
**
**  Measured distance is calculated as: end time - start time, which is then divided by 58 - constant
**  to get the distance in centimeters.
**  \n End time is the difference of the end and start time multiplier times the counter size added to
**  the measured end time. Start time is just the measured start time.
** \attention Only when rollover happens!
** \attention Adds the entire counter size to endTime to compensate for potential rollover.
**/
{
    endTime += CYCLE * (endTimeMult-startTimeMult);      				/* Adding entire 16bit counter value to end time to compensate for rollover. */
    distance = (endTime - startTime)/CMCONST;
	if ((endTime - startTime) > MAXDIST){
		usDataStatus = US_DATA_FALSEMAX;
	}
	else if((endTime - startTime) < MINDIST){
		usDataStatus = US_DATA_FALSEMIN;
	}
	else{
		usDataStatus = US_DATA_TRUE;
	}
}

/*********************************************************/
void 
resetTimes()
/**  Reset start and end time to 0 to insure proper results.
**  \attention Private function.
**/
{
	startTime = 0; 														/* Setting up for the next measurement. */
	endTime = 0;														/* Set end time also 0 in case we don't catch interrupt.  */
}

/*********************************************************/
void 
ultraS_cyclic()
/** Ultrasonic modules main finite state machine.
**
**  Controls the work regarding the ultrasonic hardware. Finite state machine to decide state of system.
**  \attention US_WORKING state is further controlled by another state machine.
**/
{
    switch(usStatus){
    case US_OK:
        if(usDataStatus == US_DATA_READ){
            usStatus = US_IDLE;        	                            	/* Going back to waiting for work. */
            resetTimes();
        }
        break;

    case US_WORKING:                     	                        	/* Figure out how busy we actually are. / FSM2 */

        switch(usDataStatus){
        case US_DATA_FALSEMIN:
            usStatus = US_ERROR;
            break;

        case US_DATA_FALSEMAX:
            usStatus = US_ERROR;
            break;

        case US_DATA_TRUE:
            usStatus = US_OK;
            break;

        case US_DATA_TRUE2:
            ultraS_prepInfo();
            break;

        case US_DATA_FALSE:
            __no_operation();
            break;
        case US_DATA_READ:
            __no_operation();
            break;
        }
    case US_ERROR:                     		                            /* Let higher level know we're done */
        usStatus = US_OK;
        break;

    case US_IDLE:                       		                        /* Waiting for work */
        __no_operation();
        break;
    }
}

/*********************************************************/
unsigned int 
ultraS_getValidStatus()
/** Get ultrasonic modules state.
** 
** \return usStatus
**/
{
	return usStatus;
}

/*********************************************************/
void
ultraS_setValidStatus(
			enum status validStatus)     /**< Status to set */
/** Set ultrasonic modules state.
**/
{
	usStatus = validStatus;
}

/*********************************************************/
unsigned int 
ultraS_getDistance()
/** Get measured distance.
**
** \return Distance 
*/
{										/* In reality distance can not be larger than 2 bytes, hence casting should be fine. */
	return distance;
}

/*********************************************************/
int 
ultraS_getDataStatus()
{
/** Get ultrasonic modules workings state.
**
**  Get the inner FSM's state.
** \return usDataStatus
*/
    return usDataStatus;
}

/*********************************************************/
void 
ultraS_setDataStatus(
		enum dataStatus validStatus)	/**< Status to set */
/** Set ultrasonic modules workings state.
**
**  Set the inner FSM's state.
**/
{
    usDataStatus = validStatus;
}

/*********************************************************/
#pragma vector=PORT1_VECTOR			 /*< Interrupt vector pragma for port 1. */
__interrupt 
void 
ultraS_ISR (
		void)
/** Interrupt vector for Port 1.
**
**  Is triggered when the hardware ultrasonic module sends back an echo signal.
**  \n The rising edge of the echo signal is saved as the starting time and the falling edge as the ending time.
**/
{

    if (gpio_getInterrupt(gpio_PORT_P1, gpio_PIN5)){
     	if (!startTime){													/* Rising endge. */
         	startTime = counter_getCountValue(counter_timerBase);
        	startTimeMult = counter_getOverflow();
         	gpio_setInterruptEdge (gpio_PORT_P1, gpio_PIN5,
									  gpio_HIGH_TO_LOW_TRANSITION);
     	}
        else if (startTime && !endTime ){									/* Falling edge. */
     		endTime = counter_getCountValue(counter_timerBase);
     		endTimeMult = counter_getOverflow();
     		gpio_setInterruptEdge (gpio_PORT_P1, gpio_PIN5,
     							      gpio_LOW_TO_HIGH_TRANSITION);
            usDataStatus = US_DATA_TRUE2;									/* In module status. */
        }
    }

    gpio_clearInterrupt ( gpio_PORT_P1, gpio_PIN5 );
}
