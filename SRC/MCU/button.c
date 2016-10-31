/*
 * button.c
 *
 *  Created on: 31. okt 2016
 *      Author: Y009
 */
//***** Header Files **********************************************************
#include <driverlib.h>
#include "button.h"

//***** DEFINES ***************************************************************
#define NOTDEBOUNCED 0xFF
#define DEBOUNCED 0
#define ON 1
#define OFF 0
unsigned volatile int unbouncedBTN = 0;
unsigned volatile int BTN = 0;				/* Debounced button value */
unsigned long int lastCounter = 0;			/*  */

//unsigned volatile int Data[100]; 			// Testing debounce
//unsigned volatile int i = 0;

int getBtn (void){
  return BTN;
}

void debounceBTN(){
	diTAI();
	if ((lastCounter + getCounter()) >  0x20){	/* Debouncing pin 1.1 (on board button) */
		unbouncedBTN <<= 1;
		unbouncedBTN |= GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)&0x01;
		if(unbouncedBTN == NOTDEBOUNCED)
			BTN = OFF;
		else if(unbouncedBTN == DEBOUNCED)
			BTN = ON;
		lastCounter = getCounter();
		//Data[i] = unbouncedBTN;
		//i++;
	}
	enTAI();
}

