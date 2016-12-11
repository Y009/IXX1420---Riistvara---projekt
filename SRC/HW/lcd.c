/*
 * lcd.c
 *
 *  Created on: 20. nov 2016
 *      Author: Karl
 *      Todo: merge PUHASTA and ROW_CHANGE states, make universal state for command/special command sending, idk if possible, make a stuct for it/
 *		fix: holding RST results int the lcd being flooded
 *
 */
#include "lcd.h"
#include "uart.h"
#include "counter.h"
#define DELAY 10000000
#define TEST 1

uint8_t strMeasurement[MAX_LEN];
uint8_t MOVE_CURSOR[2] = {COMMAND, 0xC0};
uint8_t DISPLAY[2] = {COMMAND, CLEAR_DISPLAY};
unsigned long long int SEC = 0;

enum states state = WAIT_STATE;

int i;
int e=0;
int f=0;
int flag =0;
int puhasta_flag= 1;
int REAL_LEN =0;
int MAX_ROW = 16;
int row_flag=0;

#ifdef TEST
    int test1;
    int test2;
    int test3;
#endif /* MACRO */


/*	returns the current state of the machine
 *  ToDo: return value constants
 */
short lcd_getState(void){
    	if (state == WAIT_STATE){
          return IDLE;
    	}else{
    		return 0;
    	}
    }


/* sends string to display, maximum string length: 32 characters
 * ToDo: string length check
 * 		 ASCII character bound checking (we won't let user display everything)
 * 		 fix counter names
 */

int lcd_sendString(char *e){

	if(lcd_initStateMachine() ==1){

	for(i=0; i<MAX_LEN+1; i++){

		if(i>=MAX_ROW && row_flag ==0){
			row_flag=1; /*you need to reset it after changing cursor row*/
		}

		if((uint8_t)(*(e+i))=='\0'){  /*if its not 16 char then it tells the lcd to change row */
			REAL_LEN = i;
			break;

	}

		*(strMeasurement + i) = (uint8_t)(*(e+i)); /*no need for sign, type cast*/
	}

	i=0; //
	flag = 1;
	return 1;
	}
	return 0;
}
/*
 * ToDo: proper return constants (no magic numbers)
 */
int lcd_initStateMachine(){

if(lcd_getState() == IDLE){
	state = IDLES; 					/*initial state*/
	i=0;							/*counter for string displaying(name needs fixing), max value defined in lcd.h as MAX_LEN(16)*/
	SEC = counter_getOverflow();
	puhasta_flag= 1; /* add if clauses for string type, which one to clear*/
	return 1;
	}
	return 0;
}


/* Todo: proper return values for flow control and flag setting(will cue two consecutive string)
 *
 *
 */
int lcd_cyclic(){

	switch(state){

	case WAIT_STATE:

		if ((counter_getOverflow() - SEC) == 77 /*&& puhasta_flag == 1*/){
			state= PUHASTA;
			puhasta_flag = 0;
			break;
		}


		state = WAIT_STATE;
		break;

	case IDLES: /* find a more appropriate name for the state */

		if(i < REAL_LEN){ /*check if any data needs to be transmitted */
			flag = 1;	/*set the appropriate flag to indicate transmission, name needs fixing */
		}else{
			flag=0;
			/*puhasta_flag = 1;*/
			state = WAIT_STATE;
		}

		if(flag==1 && puhasta_flag == 1){
			/*puhasta_flag = 1;*/
			state = PUHASTA;	/*state = BUSYS;*/
			break;
		}else if (flag ==1 ){
			state = BUSYS;
			break;
		}

		//__delay_cycles(DELAY/10);
		state = WAIT_STATE;
		break;

	case BUSYS:

		if(UART_sendByte(strMeasurement[i]) == SUCCESS){
			state = IDLES;
			i++;
			break;
		}else{state = IDLES;}

		if(i == MAX_ROW && row_flag ==1){
			state = ROW_CHANGE;
		}
		break;

	case ROW_CHANGE:


		if(UART_sendByte(MOVE_CURSOR[e]) == SUCCESS){
					state = ROW_CHANGE;
					e++;
					test1++;
					break;

				}else{
					test2++;
					state = ROW_CHANGE;}

			if (e > 1){
					test3++;
					state = BUSYS;
					row_flag =0;
					break;
				}

	case PUHASTA:

			if(UART_sendByte(DISPLAY[f]) == SUCCESS){
								state = PUHASTA;
								f++;
								break;

							}else{
								state = PUHASTA;}

						if (f > 1){
							state = IDLES;
							puhasta_flag = 0;
							f = 0;
							break;

							}
	}

	return 1;
}
