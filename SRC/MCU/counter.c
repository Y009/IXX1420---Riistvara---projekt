 /*
 * counter.c
 *
 *  Created on: 31. okt 2016
 *      Author: Y009
 *
 *      Free running timer for ultrasonic module interrupt.
 */

//***** Header Files **********************************************************
#include <driverlib.h>
#include "counter.h"

//***** DEFINES ***************************************************************

#define TIMETO 0xFDE8						/* 32.5 ms cycle for timer B.*/
unsigned long long int timerB_overflow;      /* Maximum time; Will only result in false data when it overflow itself. 32.77ms * 2^32 = 4.46 years */

unsigned long long int counter_getOverflow(void){
  return timerB_overflow;
}

void counter_init(void){
    Timer_B_initUpModeParam initUpParam = { 0 };
        initUpParam.clockSource = TIMER_B_CLOCKSOURCE_SMCLK;                      /* Use SMCLK (faster clock) */
        initUpParam.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_16;           /* Input clock = SMCLK / 1 = 16MHz */
        initUpParam.timerPeriod = TIMETO;                                    	  /* 32.5ms - dec: 65 000*/
        initUpParam.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_ENABLE;    /* Enable TAR -> 0 interrupt */
        initUpParam.captureCompareInterruptEnable_CCR0_CCIE =
                TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;                               /* Enable CCR0 compare interrupt */
        initUpParam.timerClear = TIMER_B_DO_CLEAR;                                /* Clear TAR & clock divider */
        initUpParam.startTimer = false;                                           /* Don't start the timer, yet */
    Timer_B_initUpMode( TIMER_B0_BASE, &initUpParam );                            /* Set up Timer A0 */

    //*************************************************************************
    //  Clear/enable interrupt flags and start timer
    //*************************************************************************
    Timer_B_clearTimerInterrupt( TIMER_B0_BASE );                                 /* Clear TA0IFG */
    Timer_B_clearCaptureCompareInterrupt( TIMER_B0_BASE,
        TIMER_B_CAPTURECOMPARE_REGISTER_0                                         /* Clear CCR0IFG interrupt pending flag bit */
    );

    Timer_B_startCounter(
        TIMER_B0_BASE,
        TIMER_B_UP_MODE
    );
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void addToTimer (void){
    timerB_overflow++;
    Timer_B_clearTimerInterrupt(TIMER_B0_BASE);										/* Clear TB0IFG */
}
