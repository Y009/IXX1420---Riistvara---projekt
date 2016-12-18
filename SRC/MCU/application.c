/*
 * application.c
 *
 *  Created on: 4. dets 2016
 *      Author: Y009
 */

/** \file application.c
**	\brief Documentation for the application module.
**
**  The application module serves as an intermediary between modules to handle work sequence.
**/

#include "timer.h"
#include "../HW/button.h"
#include "../HW/ultraS.h"
#include "../HW/lcd.h"
#include "uart.h"
#include "../MCU/counter.h"
#include <stdio.h>

#define DELAY 10000U							/**< 0.000625 second delay */

void convertUDec(unsigned long n);
unsigned long long int lastCounter2 =77;
unsigned long int distance;						/**< Local variable of distance to be converted to string */

char String[16] = "Kaugus : ";					/**< String to be displayed on the LCD \b after the distance has been added to it */
char StringError[16] = "Error!         ";		/**< String to be displayed on the LCD \b if the distance fell out of the measurable range */

void 
application_cyclic()
/**	The main cyclic function to control the workflow.
**
**	Initiates check on button.
**	\n If the button has been debounced starts the work (if not already in progress.)
**	\n Clears the display and checks with ultrasonic module.
**	\n Sends a string to the LCD module to be displayed based on value from the ultrasonic module.
**/
{

	timer_checkFlag();
	button_debounceBtn();

    if(button_getBtn() && (ultraS_getValidStatus() != US_WORKING)){
    	ultraS_sendSignal();
    }

    if (ultraS_getValidStatus() == US_OK /*&& ready*/) {

    	if (ultraS_getDataStatus() == (US_DATA_FALSEMAX || US_DATA_FALSEMIN || US_DATA_FALSE)){

   		if(lcd_getState() == lcd_IDLE)
   		{
    			lcd_sendString(StringError);

   		}

    	}
    	else {
    		distance = ultraS_getDistance();            	/* To be actually given to the lcd module */
            convertUDec(distance);

        	if(lcd_getState() == lcd_IDLE)
        	{
        		lcd_sendString(String);
        	}
           // UART_sendByte(0x42);



    	}

		ultraS_setDataStatus(US_DATA_READ);
    }
}

void
convertUDec(
		unsigned long n)						/**< Numeric value to be converted to string. */
/**	Converts integer value to string
**
**	Converts integer to string and places it in \b String to the appropriate position
**  \n Converts each digit of the value on its own by taking modulo 10 of it and adding a value of 30 to it.
**	\n Based on the ascii table where numbers are from 30 to 39.
**
** \attention Currently limited to 4 digit numbers.
**/
{
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

