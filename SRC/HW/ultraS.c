/*
 * ultraS.c
 *
 *  Created on: 7. nov 2016
 *      Author: Y009
 */

//***** Header Files **********************************************************
#include <driverlib.h>
#include "ultraS.h"
#include "../MCU/counter.h"
#include "../MCU/timer.h"
s#include "stdio.h"
#include "gpio.h"

//***** DEFINES ***************************************************************
#define MAXDIST 23202							/* 23200us is 4m. Giving a little error time. */
#define MINDIST 114								/* 116us is 2cm. Giving a little error time. */
#define CMCONST 58								/* Constant to divide time with to get distance in centimeters. */
#define CYCLE 0xFDE8

void resetTimes();

unsigned volatile long long int startTime;
unsigned volatile long long int endTime;
unsigned volatile long long int startTimeMult;
unsigned volatile long long int endTimeMult;
unsigned volatile long int distance;

enum status usStatus = US_IDLE;
enum dataStatus usDataStatus = US_DATA_FALSE;

void ultraS_init(){
    usStatus = US_WORKING;
    resetTimes();
    usStatus = US_IDLE;
}

void ultraS_sendSignal(){
    usStatus = US_WORKING;
	gpio_setPinHigh (gpio_PORT_P1, gpio_PIN4);
	__delay_cycles(160U);												/* trigger time */
	gpio_setPinLow (gpio_PORT_P1, gpio_PIN4);
	__delay_cycles(96000U);												/* echo wait period */
}

void ultraS_prepInfo(){													/* Main can check before getting distance, whether data is actually valid. */
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

void resetTimes(){
	startTime = 0; 														/* Setting up for the next measurement. */
	endTime = 0;														/* Set end time also 0 in case we don't catch interrupt.  */
}

void ultraS_cyclic(){
    switch(usStatus){
    case US_OK:
        if(usDataStatus == US_DATA_READ){
            usStatus = US_IDLE;        		/* Going back to waiting for work. */
            resetTimes();
        }
        break;

    case US_WORKING:                     	 	/* Figure out how busy we actually are. */

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
    case US_ERROR:                     		/* Let higher level know we're done */
        usStatus = US_OK;
        break;

    case US_IDLE:                       		/* Waiting for work */
        __no_operation();
        break;
    }
}


/** Getters, Setters  **/

unsigned int ultraS_getValidStatus(){
	return usStatus;
}
void ultraS_setValidStatus(enum status validStatus){
	usStatus = validStatus;
}

unsigned int ultraS_getDistance(){										/* In reality distance can not be larger than 2 bytes, hence casting should be fine. */
	return distance;
}

int ultraS_getDataStatus(){
    return usDataStatus;
}

void ultraS_setDataStatus(enum dataStatus validStatus){
    usDataStatus = validStatus;
}

/** Getters, Setters  **/


#pragma vector=PORT1_VECTOR
__interrupt void ultraS_ISR (void){

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
            usDataStatus = US_DATA_TRUE2;											/* In module status. */
        }
    }

    gpio_clearInterrupt ( gpio_PORT_P1, gpio_PIN5 );
}
