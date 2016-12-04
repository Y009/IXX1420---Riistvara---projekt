/*
 * gpioInit.c
 *
 *  Created on: 16. okt 2016
 *      Author: Y009
 */

//***** Header Files **********************************************************
#include <driverlib.h>
#include "gpio.h"

void gpio_init(){
	P3SEL |= BIT3; 														/* UART pin select */
	P5SEL |= BIT2 + BIT3;  												/* XT2CLK pin select */

    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P2, GPIO_PIN1);     /* Pin 1.1 as Input for on board button */
	GPIO_setAsOutputPin (GPIO_PORT_P1, GPIO_PIN4);                      /* Pin 1.4 as Output for ultrasonic module */
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN5);     /* Pin 1.5 as Input for ultrasonic module */
    GPIO_selectInterruptEdge (GPIO_PORT_P1, GPIO_PIN5,
                              GPIO_LOW_TO_HIGH_TRANSITION);             /* Initially set interrupts up for rising edge signal */
    GPIO_clearInterrupt (GPIO_PORT_P1, GPIO_PIN5);                      /* Clear interrupts before enabling them */
    GPIO_enableInterrupt (GPIO_PORT_P1, GPIO_PIN5);  	                /* Pin 1.5 input interrupt for ultrasonic module data */
}
