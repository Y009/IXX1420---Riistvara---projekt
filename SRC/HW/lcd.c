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
**  Describes the workings of the lcd state machine.
**/

//***** Header Files **********************************************************
#include "lcd.h"
#include "../MCU/uart.h"
#include "../MCU/counter.h"

//***** DEFINES ***************************************************************

uint8_t strMeasurement[MAX_LEN];
uint8_t MOVE_CURSOR[2] = { COMMAND, 0xC0 };
uint8_t DISPLAY[2] = { COMMAND, CLEAR_DISPLAY };
unsigned long long int SEC = 0;

lcd_states state = LCD_WAIT_STATE;

int i;
int e = 0;
int f = 0;
int flag = 0;
int trans_flag=1;
int puhasta_flag = 1;
int REAL_LEN = 0;
int MAX_ROW = 16;
int row_flag = 0;

/*********************************************************/

/*	returns the current state of the machine
 *  ToDo: return value constants
 */
short
lcd_getState(void)
/** Get current status of the machine
**
**  \return lcd_IDLE
**  \return lcd_BUSY
**/
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

int lcd_sendString(char *e)
{
	SEC = counter_getOverflow();
	if (lcd_initStateMachine() == 1)
	{
		trans_flag=0;
		for (i = 0; i < MAX_LEN + 1; i++)
		{

			if (i > MAX_ROW && row_flag == 0)
			{
				row_flag = 1;
			}

			if ((uint8_t)(*(e + i)) == '\0')
			{
				REAL_LEN = i;
				break;

			}

			*(strMeasurement + i) = (uint8_t)(*(e + i));
		}

		i = 0; /*reset character counter */
		flag = 1; /* set flag indicating need for transmission */
		return 1;
	}
	return 0;
}
/*
 * ToDo: proper return constants (no magic numbers)
 */
int lcd_initStateMachine()
{
	if (lcd_getState() == lcd_IDLE)
	{
		state = LCD_IDLE; 					/*initial state*/
		i = 0;							/*counter for string displaying(name needs fixing), max value defined in lcd.h as MAX_LEN(32)*/
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
int lcd_cyclic()
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

		if (i < REAL_LEN)	/*check if any data still needs to be transmitted */
		{
			flag = 1;	/*set the appropriate flag to indicate transmission */

		} else
		{
			flag = 0;
			state = LCD_WAIT_STATE;
			break;
		}
		if (puhasta_flag == 1)
		{
			state = LCD_CLEAR_DISPLAY;
			break;
		} else if (flag == 1) /*data to be transmitted */
		{
			state = LCD_BUSY;
			break;
		}

		state = LCD_WAIT_STATE; /*no data to transmit nor no cleaning to do */
		break;

	case LCD_BUSY:

		if (UART_sendByte(strMeasurement[i]) == uart_SUCCESS)
		{
			state = LCD_IDLE;
			i++;
			break;
		} else { state = LCD_IDLE; } /*in case of failure go check flags again */

		if (i == MAX_ROW && row_flag == 1) /* if last character reached and there's a need for row change */
		{
			state = LCD_ROW_CHANGE;
		}
		break;

	case LCD_ROW_CHANGE:


		if (UART_sendByte(MOVE_CURSOR[e]) == uart_SUCCESS)
		{
			state = LCD_ROW_CHANGE;
			e++;
			break;

		} else
		{
			state = LCD_ROW_CHANGE;
		}

		if (e > 1)
		{
			state = LCD_BUSY;
			row_flag = 0;
			break;
		}

	case LCD_CLEAR_DISPLAY:

		if (UART_sendByte(DISPLAY[f]) == uart_SUCCESS)
		{
			state = LCD_CLEAR_DISPLAY;
			f++;


		} else
		{
			state = LCD_CLEAR_DISPLAY;
		}

		if (f > 1)
		{
			state = LCD_IDLE;
			puhasta_flag = 0;
			flag=0;
			f = 0;
			break;

		}
	}

	return 1;
}
