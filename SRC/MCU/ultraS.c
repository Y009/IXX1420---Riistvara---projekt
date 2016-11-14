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

unsigned long int startTime;
unsigned long int endTime;
unsigned long int distance;
char valid;

void ultraS_init(){
	startTime = 0;
	endTime = 0;
    GPIO_setAsOutputPin (GPIO_PORT_P1, GPIO_PIN4);                      /* Pin 1.4 as Output for ultrasonic module */
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN5);     /* Pin 1.5 as Input for ultrasonic module */
    GPIO_selectInterruptEdge (GPIO_PORT_P1, GPIO_PIN5,
                              GPIO_LOW_TO_HIGH_TRANSITION);             /* Initially set interrupts up for rising edge signal */
    GPIO_clearInterrupt (GPIO_PORT_P1, GPIO_PIN5);                      /* Clear interrupts before enabling them */
    GPIO_enableInterrupt (GPIO_PORT_P1, GPIO_PIN5);  	                /* Pin 1.5 input interrupt for ultrasonic module data */

}

void ultraS_sendSignal(){
	GPIO_setOutputHighOnPin (GPIO_PORT_P1, GPIO_PIN4);
	__delay_cycles(160);												/* 10uS; just testing before making a timer or leave it? */
	GPIO_setOutputLowOnPin (GPIO_PORT_P1, GPIO_PIN4);
}

void ultraS_prepInfo(){													/* Main can check before getting distance, whether data is actually valid. */

	if ((endTime - startTime) > maxDist){
		valid = FalseMax;
	}
	else if((endTime - startTime) < minDist){
		valid = FalseMin;
	}
	else{
		distance = (endTime - startTime)/cmConst;
		valid = True;
	}

	startTime = 0; 														/* Setting up for the next measurement. */
	endTime = 0;														/* Set end time also 0 in case we don't catch interrupt.  */
}

char ultraS_getValidStatus(){
	return valid;
}

unsigned int ultraS_getDistance(){										/* In reality distance can not be larger than 2 bytes, hence casting should be fine. */
	return distance;
}

#pragma vector=PORT1_VECTOR
__interrupt void pushbutton_ISR (void){

	 if (GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN5)){
     	if (!startTime){
     		timer_diTBI();
         	startTime = counter_getCounter();
         	timer_enTBI();
             GPIO_selectInterruptEdge (GPIO_PORT_P1, GPIO_PIN5,
                                       GPIO_HIGH_TO_LOW_TRANSITION);
     	}
     	else{
     		timer_diTBI();
     		endTime = counter_getCounter();
     		timer_enTBI();
     	    GPIO_selectInterruptEdge (GPIO_PORT_P1, GPIO_PIN5,
     	                              GPIO_LOW_TO_HIGH_TRANSITION);
     	}
	 }
}
