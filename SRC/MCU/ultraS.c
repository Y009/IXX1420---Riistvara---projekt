/*
 * ultraS.c
 *
 *  Created on: 7. nov 2016
 *      Author: Y009
 */

//***** Header Files **********************************************************
#include <driverlib.h>
#include "ultraS.h"
#include "counter.h"

//***** DEFINES ***************************************************************
#define maxDist 23202							/* 23200us is 4m. Giving a little error time. */
#define minDist 114								/* 116us is 2cm. Giving a little error time. */
#define cmConst 58								/* Constant to divide time with to get distance in centimeters. */

unsigned long long int startTime;
unsigned long long int endTime;
unsigned long int distance;

enum status usStatus = Idle;
enum dataStatus usDataStatus = False;

void ultraS_init(){
    usStatus = Busy;
	startTime = 0;
	endTime = 0;
    GPIO_setAsOutputPin (GPIO_PORT_P1, GPIO_PIN4);                      /* Pin 1.4 as Output for ultrasonic module */
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN5);     /* Pin 1.5 as Input for ultrasonic module */
    GPIO_selectInterruptEdge (GPIO_PORT_P1, GPIO_PIN5,
                              GPIO_LOW_TO_HIGH_TRANSITION);             /* Initially set interrupts up for rising edge signal */
    GPIO_clearInterrupt (GPIO_PORT_P1, GPIO_PIN5);                      /* Clear interrupts before enabling them */
    GPIO_enableInterrupt (GPIO_PORT_P1, GPIO_PIN5);  	                /* Pin 1.5 input interrupt for ultrasonic module data */
    usStatus = Ok;
}

void ultraS_sendSignal(){
    usStatus = Busy;
	GPIO_setOutputHighOnPin (GPIO_PORT_P1, GPIO_PIN4);
	__delay_cycles(160);												/* 10uS */
	GPIO_setOutputLowOnPin (GPIO_PORT_P1, GPIO_PIN4);
}

void ultraS_prepInfo(){													/* Main can check before getting distance, whether data is actually valid. */
    if(endTime < startTime){                                            /* Counter rolled over between getting values. */
        endTime += 0xFFFF;                                              /* Adding entire 16bit counter value to end time to compensate for rollover. */
    }
	if ((endTime - startTime) > maxDist){
		usStatus = Error;
	}
	else if((endTime - startTime) < minDist){
		usStatus = Error;
	}
	else{
		ultraS_setDistance((endTime - startTime)/cmConst);
		usDataStatus = True;
	}

	startTime = 0; 														/* Setting up for the next measurement. */
	endTime = 0;														/* Set end time also 0 in case we don't catch interrupt.  */
}

void ultraS_cyclic(){
    switch(usStatus){
    case Ok:
        if(ultraS_getDataStatus() == Read){
            usStatus = Idle;        /* Going back to waiting for work. */
        }
        break;

    case Busy:                      /* Figure out how busy we actually are. */

        switch(usDataStatus){
        case FalseMin:
            usStatus = Error;
            break;

        case FalseMax:
            usStatus = Error;
            break;

        case True:
            usStatus = Ok;
            break;

        case True2:
            ultraS_prepInfo();
            break;

        case False:
            __no_operation();
            break;
        case Read:
            __no_operation();
            break;
        }
    case Error:                     /* Let higher level know we're done */
        usStatus = Ok;
        break;

    case Idle:                       /* Waiting for work */
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
void ultraS_setDistance(unsigned long int dist){
	distance = dist;
}
/*                                                          ei tohiks vajalikud olla kuna ainult siin kasutatavad.
void ultraS_setStartTime(time){
    startTime = time;
}
unsigned long int ultraS_getStartTime(){
    return startTime;
}

void ultraS_setEndTime(time){
    endTime = time;
}
unsigned long int ultraS_getEndTime(){
    return endTime;
}

enum ultraS_getStatus(){
    return usStatus;
}
*/
int ultraS_getDataStatus(){
    return usDataStatus;
}

void ultraS_setDataStatus(enum dataStatus validStatus){
    usDataStatus = validStatus;
}

/** Getters, Setters  **/


#pragma vector=PORT1_VECTOR
__interrupt void pushbutton_ISR (void){

    if (GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN5)){
     	if (!startTime){
     		//timer_diTBI();                                         // nestimine pole peal nii kui nii
         	startTime = Timer_B_getCounterValue(TIMER_B0_BASE);
         	//timer_enTBI();
            GPIO_selectInterruptEdge (GPIO_PORT_P1, GPIO_PIN5,
                                       GPIO_HIGH_TO_LOW_TRANSITION);
     	}
        else{
     		//timer_diTBI();
     		endTime = Timer_B_getCounterValue(TIMER_B0_BASE);
     		//timer_enTBI();
     	    GPIO_selectInterruptEdge (GPIO_PORT_P1, GPIO_PIN5,
     	                              GPIO_LOW_TO_HIGH_TRANSITION);
            usDataStatus = True2;									/* In module notification. */
        }
    }
    GPIO_clearInterrupt ( GPIO_PORT_P1, GPIO_PIN1 );
}
