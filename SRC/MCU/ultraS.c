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
unsigned long int startTime;
unsigned long int endTime;

void ultraS_init(){
	startTime = 0;
	endTime = 0;
    GPIO_setAsOutputPin (GPIO_PORT_P1, GPIO_PIN4);                      //Pin 1.4 as Output for ultrasonic module
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN5);     //Pin 1.5 as Input for ultrasonic module
    GPIO_selectInterruptEdge (GPIO_PORT_P1, GPIO_PIN5,
                              GPIO_LOW_TO_HIGH_TRANSITION);             //Initially set interrupts up for rising edge signal
    GPIO_clearInterrupt (GPIO_PORT_P1, GPIO_PIN5);                      //Clear interrupts before enabling them
    GPIO_enableInterrupt (GPIO_PORT_P1, GPIO_PIN5);  	                //Pin 1.5 input interrupt for ultrasonic module data

}

void ultraS_sendSignal(){
	GPIO_setOutputHighOnPin (GPIO_PORT_P1, GPIO_PIN4);
	__delay_cycles(160);												/* 10uS; just testing before making a timer or leave it? */
	GPIO_setOutputLowOnPin (GPIO_PORT_P1, GPIO_PIN4);
}

void ultraS_prepInfo(){
	/* endtime - starttime
	 * korrutada konstandiga, et saada cm
	 * saata k�rgemale/teha stringiks
	 * 		mitte voidiks et returinda?
	 *
	 *
	 * */

	startTime = 0; 														/* setting up for the next measurement */
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

    /*switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
        case P1IV_NONE:   break;                                // None
        case P1IV_P1IFG0:                                       // Pin 0
        	__no_operation();
             break;
        case P1IV_P1IFG1:                                       // Pin 1 (button 2)
        	__no_operation();
           	break;
        case P1IV_P1IFG2:                                       // Pin 2
        	__no_operation();
            break;
        case P1IV_P1IFG3:                                       // Pin 3
        	__no_operation();
            break;

        case P1IV_P1IFG4:                                       // Pin 4
        	__no_operation();
            break;

        case P1IV_P1IFG5:                                       // Pin 5
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
        	}
            break;

        case P1IV_P1IFG6:                                       // Pin 6
        	__no_operation();
            break;
        case P1IV_P1IFG7:                                       // Pin 7
        	__no_operation();
            break;
        default:   __never_executed();
    }
    */
}