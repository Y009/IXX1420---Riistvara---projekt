/*
 * lcd.h
 *
 *  Created on: 17. okt 2016
 *      Author: Karl
 *      Todo: move stdint.h include?
 */

/** \file lcd.h
**	\brief Documentation for the lcd's header.
**
**  Global enumeration defines.
**/

#ifndef LCD_H_
#define LCD_H_
#include <stdint.h>

#define MAX_LEN 32 /**< Maximum number of characters that fits on the display */
typedef enum
{
	LCD_IDLE,	/**< No string transfer in progress, checks for flags */
	LCD_BUSY,	/**< String transfer in progress */
	LCD_WAIT_STATE,	/**< Waiting for timeout */
	LCD_CLEAR_DISPLAY,	/**< Clearing lcd display */
	LCD_NUMBER_OF_TYPES	
}lcd_states;

#define lcd_IDLE 1 /**< LCD status define */
#define lcd_BUSY 0 /**< LCD status define */

/** \cond IGNORE*/
//special commands
#define SPECIAL_COMMAND 0x7C
#define MAXIMUM_BACKLIGHT 0x9D
#define MINIMUM_BACKLIGHT 0x80
#define PRECENT_BACKLIGHT 0x96

//commands
#define COMMAND 0xFE
#define CLEAR_DISPLAY 0x01
#define MV_LEFT 0x10
#define MV_RIGTH 0x14
#define VISUAL_DISPLAY_ON 0x0C
#define UNDERLINE_CURSOR_ON 0x0C
#define BLINKING_BOX_ON 0x0D


int lcd_initStateMachine();
void lcd_cyclic();
int lcd_sendString(char *i);
short lcd_getState(void);
/** \endcond IGNORE*/

#endif /* LCD_H_ */
