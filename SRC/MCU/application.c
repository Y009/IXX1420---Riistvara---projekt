/*
 * application.c
 *
 *  Created on: 4. dets 2016
 *      Author: Y009
 */
#include "timer.h"
#include "button.h"
#include "ultraS.h"
#include "lcd.h"
#include "uart.h"
#include <stdio.h>

#define DELAY 10000000 //0.625 second delay

void convertUDec(unsigned long n);

unsigned long int dist = 0;

char String[16] = "Kaugus : ";
char StringError[16] = "Error!         ";

void application_cyclic(){
	timer_checkFlag();
	button_debounceBtn();

    if(button_getBtn() && (ultraS_getValidStatus() != US_WORKING)){
    	ultraS_sendSignal();
    }

    if (ultraS_getValidStatus() == US_OK) {

    	UART_sendByte(COMMAND);
		__delay_cycles(DELAY/1000);
		UART_sendByte(CLEAR_DISPLAY);

    	if (ultraS_getDataStatus() == (US_DATA_FALSEMAX || US_DATA_FALSEMIN)){
            lcd_sendString(StringError);
    	}
    	else {
        	dist = ultraS_getDistance();            	/* To be actually given to the lcd module */
            convertUDec(dist);
    		lcd_sendString(String);
    	}

		ultraS_setDataStatus(US_DATA_READ);
    }
}

void convertUDec(unsigned long n){
	int i;
    for(i = 12; i >= 9; i--){
        if((n > 0) || (i == 12))    			/*  Has to have at least 1 digit to have made here or at least something left to convert */
		String[i] = n % 10 + 0x30;  			/*  Numbers in ascii: 30 - 39 */
        else                      				/*  Add spaces if no more digits */
            String[i] = ' ';
        n = n / 10;                				/*  New digit to use */
    }

    String[13] = 'c';
    String[14] = 'm';
    String[15] = 0;
}

