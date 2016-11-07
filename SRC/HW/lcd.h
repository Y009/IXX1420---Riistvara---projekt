/*
 * lcd.h
 *
 *  Created on: 17. okt 2016
 *      Author: Karl
 */

#ifndef LCD_H_
#define LCD_H_

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

//ASCII CHARACTERS
#define ASCII_A 0x41
#define SPACE 0x20




#endif /* LCD_H_ */
