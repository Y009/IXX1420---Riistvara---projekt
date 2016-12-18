/*
 * lcd.h
 *
 *  Created on: 17. okt 2016
 *      Author: Karl
 *      Todo: move stdint.h include?
 */

#ifndef LCD_H_
#define LCD_H_

#define MAX_LEN 32 /* maximum number of characters that fits a row */
typedef enum
{
	LCD_IDLE,
	LCD_BUSY,
	LCD_ROW_CHANGE,
	LCD_WAIT_STATE,
	LCD_CLEAR_DISPLAY,
	LCD_NUMBER_OF_TYPES
}lcd_states;

#include <stdint.h>

#define lcd_IDLE 1
#define lcd_BUSY 0

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
int lcd_cyclic();
int lcd_sendString(char *i);
short lcd_getState(void);


#endif /* LCD_H_ */
