/*
 * uart.h
 *
 *  Created on: 6. okt 2016
 *      Author: Karl
 */

/** \file uart.h
**	\brief Documentation for the lcd's header.
**
**  Global constant defines.
**/
#ifndef UART_H_
#define UART_H_
#define uart_SUCCESS 1 /**< constant for uart workflow control */
#define uart_FAILURE 0 /**< constant for uart workflow control  */
#include "driverlib.h"
/** \cond IGNORE*/
void UART_init();
int UART_sendByte(uint8_t transmitData);
uint8_t UART_reciveByte();
/** \endcond IGNORE*/
#endif /* UART_H_ */
