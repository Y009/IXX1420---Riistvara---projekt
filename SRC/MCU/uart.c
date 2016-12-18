/*
 * uart.c
 *
 *  Created on: 6. okt 2016
 *      Author: Karl
 */

#include "uart.h"

/** \file uart.c
**	\brief Documentation for the UART module.
**
**  Basic initialisation of UART peripheral.
**/

#define UART_PRESCALE 0x0682  /**< constant for clockPrescalar variable in USCI_A_UART_initParam construct, see family guide for further info  */
#define UART_FIRST_MOD_REG 0x00 /**< constant for firstModReg variable in USCI_A_UART_initParam construct, see family guide for further info  */
#define UART_SECOND_MOD_REG 0x06 /**< constant for secondModReg variable in USCI_A_UART_initParam construct, see family guide for further info  */

uint8_t reciveData = 0; /**< variable for byte transfer  */

/*********************************************************/
void 
UART_init()
/** Basic UART peripheral initialisation for 9600 baud rate
**
**  
********************************************************/
{
	    USCI_A_UART_initParam param = {0};
	    param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
	    param.clockPrescalar = UART_PRESCALE;
	    param.firstModReg = UART_FIRST_MOD_REG;
	    param.secondModReg = UART_SECOND_MOD_REG;
	    param.parity = USCI_A_UART_NO_PARITY;
	    param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
	    param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
	    param.uartMode = USCI_A_UART_MODE;
	    param.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;
        
	    if(STATUS_FAIL == USCI_A_UART_init(USCI_A0_BASE, &param))
	    {
	        return;
	    }

	    USCI_A_UART_enable(USCI_A0_BASE);

}

/*********************************************************/
int 
UART_sendByte(
uint8_t transmitData)  /**< byte to be sent in hexadecimal */
/** cyclic byte sending using UART
**\return uart_SUCCESS on successful byte transfer
**\return uart_FAILURE on failed byte transfer
********************************************************/
{
    

     if( USCI_A_UART_getInterruptStatus(USCI_A0_BASE,
             USCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == UCTXIFG ){

    	 UCA0TXBUF = transmitData;

    	 return (uart_SUCCESS);
     }

     return (uart_FAILURE);
}

/*********************************************************/
uint8_t 
UART_reciveByte()
/** cyclic byte recieving using UART
**\return recived byte on success
**\return uart_FAILURE on failed byte transfer
********************************************************/
{
    
    if( USCI_A_UART_getInterruptStatus(USCI_A0_BASE,
             USCI_A_UART_RECEIVE_INTERRUPT_FLAG) == UCRXIFG ){

        reciveData = UCA0RXBUF;

    	return reciveData;
     }
     
    return (uart_FAILURE);
    
}


