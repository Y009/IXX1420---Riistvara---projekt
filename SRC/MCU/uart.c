/*
 * uart.c
 *
 *  Created on: 6. okt 2016
 *      Author: Karl
 */

#include "uart.h"

//*****************************************************************************
//
// UART baud rate defines, taken from MSP430F5x family guide
//
//*****************************************************************************
#define UART_PRESCALE 0x0682 //setting for 16MHz clock
#define UART_FIRST_MOD_REG 0x00
#define UART_SECOND_MOD_REG 0x06

void UART_init(){
        
        P3SEL |= BIT3; //UART pin select for
        
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

int UART_sendByte(uint8_t transmitData){
    
     //USCI_A_UART_transmitData(USCI_A0_BASE,
     //                            transmitData);

     if( USCI_A_UART_getInterruptStatus(USCI_A0_BASE,
                                        USCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == UCTXIFG ){

    	 UCA0TXBUF = transmitData;

    	 return (SUCCESS);
     }

     return (FAILURE);
}

uint8_t UART_reciveByte(){
    
    uint8_t reciveData;
    reciveData = USCI_A_UART_receiveData(USCI_A0_BASE);
    
    return reciveData;
    
}


