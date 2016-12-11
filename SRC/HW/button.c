/*
 * button.c
 *
 *  Created on: 31. okt 2016
 *      Author: Y009
 */
//***** Header Files **********************************************************
#include <driverlib.h>
#include "button.h"
#include "timer.h"
#include "gpio.h"

//***** DEFINES ***************************************************************
#define NOTDEBOUNCED 0xFF
#define DEBOUNCED 0
#define ON 1
#define OFF 0
#define ACCUARCY  0x10

volatile int unbouncedBTN = 0;
volatile short BTN = 0;										/* Debounced button value */
unsigned long long int lastCounter = 0;


short button_getBtn(void){
  return BTN;
}

void button_debounceBtn(){
	timer_diTAI();
	if ((timer_getCounter() - lastCounter) >= ACCUARCY){	/* Debouncing pin 1.1 (on board button) */
		unbouncedBTN <<= 1;
		unbouncedBTN |= gpio_getPinInput(gpio_PORT_P2, gpio_PIN1)&0x01;
		if(unbouncedBTN == NOTDEBOUNCED)
			BTN = OFF;
		else if(unbouncedBTN == DEBOUNCED)
			BTN = ON;
		lastCounter = timer_getCounter();
	}
	timer_enTAI();
}

