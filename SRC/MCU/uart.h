/*
 * uart.h
 *
 *  Created on: 6. okt 2016
 *      Author: Karl
 */

#ifndef UART_H_
#define UART_H_

#define SUCCESS 1
#define FAILURE 0

/*
#define USCI_INPUT_CLK      (8000000UL)  // in Hz
#define USCI_BAUD_RATE      (9600)

#define USCI_DIV_INT              (USCI_INPUT_CLK/USCI_BAUD_RATE)
#define USCI_BR0_VAL              (USCI_DIV_INT & 0x00FF)
#define USCI_BR1_VAL              ((USCI_DIV_INT >> 8) & 0xFF)

#define USCI_DIV_FRAC_NUMERATOR   (USCI_INPUT_CLK - (USCI_DIV_INT*USCI_BAUD_RATE))
#define USCI_DIV_FRAC_NUM_X_8     (USCI_DIV_FRAC_NUMERATOR*8)
#define USCI_DIV_FRAC_X_8         (USCI_DIV_FRAC_NUM_X_8/USCI_BAUD_RATE)

#if (((USCI_DIV_FRAC_NUM_X_8-(USCI_DIV_FRAC_X_8*USCI_BAUD_RATE))*10)/USCI_BAUD_RATE < 5)
#define USCI_BRS_VAL              (USCI_DIV_FRAC_X_8<< 1)
#else
#define USCI_BRS_VAL              ((USCI_DIV_FRAC_X_8+1)<< 1)
*/
#include "driverlib.h"
void UART_init();

int UART_sendByte(uint8_t transmitData);

uint8_t UART_reciveByte();

#endif /* UART_H_ */
