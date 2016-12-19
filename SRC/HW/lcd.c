/*
 * lcd.c
 *
 *  Created on: 20. nov 2016
 *      Author: Karl
 *      Todo: merge LCD_CLEAR_DISPLAY and LCD_ROW_CHANGE states, make universal state for command/special command sending, idk if possible, make a stuct for it/
 *		fix: holding RST results int the lcd being flooded
 *
 */
/** \file lcd.c
**	\brief Documentation for the lcd module.
**
**  Describes the workings of the lcd state machine
**
**@startuml {lcdFSM.png}
**[*] --> LCD_IDLE : Power on
**LCD_IDLE --> LCD_BUSY : transmission start
**LCD_BUSY --> LCD_BUSY : data to transmit
**LCD_BUSY --> LCD_WAIT_STATE : data transmission ended
**LCD_CLEAR_DISPLAY --> LCD_CLEAR_DISPLAY: display clearing in process
**LCD_CLEAR_DISPLAY --> LCD_WAIT_STATE: display cleared
**LCD_WAIT_STATE --> LCD_WAIT_STATE: timeout not reached
**LCD_WAIT_STATE --> LCD_CLEAR_DISPLAY: timeout reached
**LCD_WAIT_STATE --> LCD_IDLE: timeout reached and display cleared
**LCD_IDLE -> [*] : Power off
**@enduml
**/

//***** Header Files **********************************************************
#include "lcd.h"
#include "../MCU/uart.h"
#include "../MCU/counter.h"

//***** DEFINES ***************************************************************

uint8_t strMeasurement[MAX_LEN]; /**< Array for transmission ready string */
uint8_t DISPLAY[2] = { COMMAND, CLEAR_DISPLAY }; /**<Array for sending the display clean command to lcd */
unsigned long long int SEC = 0; /**< Timeout start time */

lcd_states state = LCD_WAIT_STATE; /**< Set finite state machine to idle/waiting state. */

int CHAR_COUNT; /**< index of the current character in transmission */
int CLEAR_DISPLAY_INDEX = 0; /**< Display cleaning array index  */
int DATA_TO_SEND = 0; /**< Flag to indicate if is any data to send, 0 if no  */
int trans_flag=1; /**< Flag to indicate if transmission has ended, 1 if yes */
int puhasta_flag = 1; /**< flag to indicate need for cleaning the display */
int REAL_LEN = 0; /**< length of the string to be displayed */


/*********************************************************/

/*	returns the current status of the machine
 *  ToDo: return value constants
 */
short
lcd_getState(void)
/** Get current status of the machine
**
**  \return lcd_IDLE
**  \return lcd_BUSY
*********************************************************/
{
	if (trans_flag == 0)
	{
		return lcd_BUSY;
	}
		return lcd_IDLE;
}


/* sends string to display, maximum string length: 32 characters
 * ToDo: string length check
 * 		 ASCII character bound checking (we won't let user display everything)
 * 		 fix counter names
 */
/*********************************************************/
int 
lcd_sendString(
				char *e ) /**< Pointer to the first character of the string */
/** Send a string to the display. 
**	Initializes flags for transmission. Resolves the string legth and prepares it for transit.
**
**
********************************************************/
{
	SEC = counter_getOverflow();
	if (lcd_initStateMachine() == 1 )
	{

		trans_flag=0;
		for (CHAR_COUNT = 0; CHAR_COUNT < 16 + 1; CHAR_COUNT++)
		{

			if ((uint8_t)(*(e + CHAR_COUNT)) == '\0')
			{
				REAL_LEN = CHAR_COUNT ;
				break;

			}

			*(strMeasurement + CHAR_COUNT ) = (uint8_t)(*(e + CHAR_COUNT ));
		}
		CHAR_COUNT = 0; /*reset character counter */
		DATA_TO_SEND = 1; /* set flag indicating need for transmission */

		return 1;
	}
	return 0;
}
/*
 * ToDo: proper return constants (no magic numbers)
 */
/*********************************************************/
int 
lcd_initStateMachine()
/** Initializes the FSM.
**	\return 1 on success
**	\return 0 on failure
**
********************************************************/
{
	if (lcd_getState() == lcd_IDLE )
	{

		state = LCD_IDLE; 					/*initial state*/
		CHAR_COUNT = 0;							/*counter for string displaying, max value defined in lcd.h as MAX_LEN(32)*/
		SEC = counter_getOverflow();
		puhasta_flag = 1; /* add if clauses for string type, which one to clear*/
		return 1;
	}
	return 0;
}


/* Todo: proper return values for flow control and flag setting(will cue two consecutive string)
 *
 *
 */
/*********************************************************/
void
lcd_cyclic()
/** LCD module FSM
** Handles the string displaying, screen cleaning and timeout for string display length
**
********************************************************/
{

	switch (state)
	{

	case LCD_WAIT_STATE:
		//i=0;
		if ((counter_getOverflow() - SEC) == 77)
		{
			trans_flag =1;
			puhasta_flag = 1;
			state = LCD_CLEAR_DISPLAY;
			break;
		}


		state = LCD_WAIT_STATE;
		break;

	case LCD_IDLE:

		if (CHAR_COUNT < REAL_LEN)	/*check if any data still needs to be transmitted */
		{
			DATA_TO_SEND = 1;	/*set the appropriate flag to indicate transmission */

		} else
		{
			DATA_TO_SEND = 0;
			state = LCD_WAIT_STATE;
			break;
		}
		if (puhasta_flag == 1)
		{
			state = LCD_CLEAR_DISPLAY;
			break;
		} else if (DATA_TO_SEND == 1) /*data to be transmitted */
		{
			state = LCD_BUSY;
			break;
		}

		state = LCD_WAIT_STATE; /*no data to transmit nor no cleaning to do */
		break;

	case LCD_BUSY:

		if (UART_sendByte(strMeasurement[CHAR_COUNT]) == uart_SUCCESS)
		{
			state = LCD_IDLE;
			CHAR_COUNT++;
			break;
		} else { state = LCD_IDLE; } /*in case of failure go check flags again */

		break;

	case LCD_CLEAR_DISPLAY:

		if (UART_sendByte(DISPLAY[CLEAR_DISPLAY_INDEX]) == uart_SUCCESS)
		{
			state = LCD_CLEAR_DISPLAY;
			CLEAR_DISPLAY_INDEX++;


		} else
		{
			state = LCD_CLEAR_DISPLAY;
		}

		if (CLEAR_DISPLAY_INDEX > 1)
		{
			state = LCD_IDLE;
			puhasta_flag = 0;
			DATA_TO_SEND=0;
			CLEAR_DISPLAY_INDEX = 0;
			break;

		}
	}

}
