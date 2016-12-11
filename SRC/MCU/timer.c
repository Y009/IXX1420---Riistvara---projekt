/*
 * timer.c
 *
 *  Created on: 23. okt 2016
 *      Author: Y009
 *
 *      Timer for debouncing button press.
 */


//***** Header Files **********************************************************
#include <driverlib.h>
#include "timer.h"

//***** DEFINES ***************************************************************

#define TIMETO 0x1F40						    /* 0.5 ms cycle for timer A. Calculated with 16MHz SMCLK with divider 1 */
unsigned long long int interruptCounter; 		/* Times the timer A interrupt has to taken place */

short timerFlag = 0 ; 			        	    /* Hiljem kui kõik töötab korralikult siis võib üheks lippude registriks teha. */

void timer_init(void){
    Timer_A_initUpModeParam initUpParam = { 0 };
        initUpParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;                      /* Use SMCLK (faster clock) */
        initUpParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;           /* Input clock = SMCLK / 1 = 16MHz */
        initUpParam.timerPeriod = TIMETO;                                    	  /* 0.5ms */
        initUpParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;    /* Enable TAR -> 0 interrupt */
        initUpParam.captureCompareInterruptEnable_CCR0_CCIE =
                TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;                               /* Enable CCR0 compare interrupt */
        initUpParam.timerClear = TIMER_A_DO_CLEAR;                                /* Clear TAR & clock divider */
        initUpParam.startTimer = false;                                           /* Don't start the timer, yet */
    Timer_A_initUpMode( timer_timerBase, &initUpParam );                            /* Set up Timer A0 */

    //*************************************************************************
    //  Clear/enable interrupt flags and start timer
    //*************************************************************************
    Timer_A_clearTimerInterrupt( timer_timerBase );                                 /* Clear TA0IFG */
    Timer_A_clearCaptureCompareInterrupt( timer_timerBase,
        TIMER_A_CAPTURECOMPARE_REGISTER_0                                         /* Clear CCR0IFG interrupt pending flag bit */
    );

    Timer_A_startCounter(
    	timer_timerBase,
        TIMER_A_UP_MODE
    );
}

void timer_checkFlag(){
	timer_diTAI(timer_timerBase);
	if(timerFlag){
		interruptCounter++;
		timerFlag = 0;					/* Clear timerA interrupt up flag */
	}
	timer_enTAI(timer_timerBase);
}

int timer_getCounter(void){
	return interruptCounter;
}

//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void debouncingBtn (void){
	timerFlag = 1;
    Timer_A_clearTimerInterrupt(timer_timerBase);										/* Clear TA0IFG */
}

