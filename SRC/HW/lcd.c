/*
 * lcd.c
 *
 *  Created on: 20. nov 2016
 *      Author: Karl
 *      Todo: move cursor to second row in state machine
 *
 */
#include "lcd.h"
#include "../MCU/uart.h"
#define DELAY 10000000
//#define MAX_LEN 16
uint8_t strMeasurement[MAX_LEN];
enum states state;
int i;
int flag =0;

/* sends string to display, maximum string length: 32 characters
 * ToDo: string length check
 * 		ASCII character bound checking (we won't let user display everything)
 * 		fix counter names
 */

int lcd_sendString(char *e){
	lcd_initStateMachine();

	for(i=0; i<MAX_LEN; i++){
		*(strMeasurement + i) = (uint8_t)(*(e+i)); /*no need for sign, type cast, redo with a FIFO buffer for dynamic string lenth, currently writes only the latest string*/
	}

	i=0; //
	flag = 1;
	return 1;
}
/*
 * ToDo: proper return constants (no magic numbers)
 */
int lcd_initStateMachine(){
	state = IDLES; 					/*initial state*/
	i=0;							/*counter for string displaying(name needs fixing), max value defined in lcd.h as MAX_LEN(16)*/
	return 1;
}


/* Todo: proper return values for flow control and flag setting(will cue two consecutive string)
 *
 *
 */
int lcd_cyclic(){

	switch(state){

	case IDLES: /* find a more appropriate name for the state */

		if(i<MAX_LEN){ /*check if any data needs to be transmitted */
			flag =1;	/*set the appropriate flag to indicate transmission, name needs fixing */
		}else{
			flag=0;

		}
		if(flag==1){

			state = BUSYS;

			break;
		}
		//__delay_cycles(DELAY/10);
		state = IDLES;
		break;

	case BUSYS:

		if(UART_sendByte(strMeasurement[i]) == SUCCESS){
			state = IDLES;
			i++;
			break;
		}else{state = IDLES;}

	}
	return 1;
}
