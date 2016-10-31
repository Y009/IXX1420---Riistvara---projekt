/*
 * timer.c
 *
 *  Created on: 23. okt 2016
 *      Author: Y009
 */


//***** Header Files **********************************************************
#include <driverlib.h>
#include "timer.h"

//***** DEFINES ***************************************************************

#define TIMETO 0x1F40						/* 0.5 ms cycle for timer A. Calculated with 16MHz SMCLK with divider 1 */
//#define INTERRUPTCOUNTER 40 				/* Times the timer A interrupt has to take place before going to the debounce conditional */

unsigned int timerFlag = 0 ; 				/* Teha yheks lipu registriks kuhu k]ik lipud panna ??? */
//unsigned volatile int timerCycle = INTERRUPTCOUNTER;


void timerInit(void)
{
    Timer_A_initUpModeParam initUpParam = { 0 };
        initUpParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;                      /* Use SMCLK (faster clock) */
        initUpParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;           /* Input clock = SMCLK / 1 = 16MHz */
        initUpParam.timerPeriod = TIMETO;                                    	  /* 0.5ms */
        initUpParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;    /* Enable TAR -> 0 interrupt */
        initUpParam.captureCompareInterruptEnable_CCR0_CCIE =
                TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;                               /* Enable CCR0 compare interrupt */
        initUpParam.timerClear = TIMER_A_DO_CLEAR;                                /* Clear TAR & clock divider */
        initUpParam.startTimer = false;                                           /* Don't start the timer, yet */
    Timer_A_initUpMode( TIMER_A0_BASE, &initUpParam );                            /* Set up Timer A0 */

    //*************************************************************************
    //  Clear/enable interrupt flags and start timer
    //*************************************************************************
    Timer_A_clearTimerInterrupt( TIMER_A0_BASE );                                 /* Clear TA0IFG */
    Timer_A_clearCaptureCompareInterrupt( TIMER_A0_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0                                         /* Clear CCR0IFG interrupt pending flag bit */
    );

    Timer_A_startCounter(
        TIMER_A0_BASE,
        TIMER_A_UP_MODE
    );
}

//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ccr0_ISR (void){
	timerFlag = 1;
    Timer_A_clearTimerInterrupt(TIMER_A0_BASE);										/* Clear TA0IFG */
}

void setFlag(int x){
	diTAI();
	timerFlag = x;
	enTAI();
}

int getFlag(void){
	return timerFlag;
}


