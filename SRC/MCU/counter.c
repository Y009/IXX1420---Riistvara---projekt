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

//#define TIMETO 0x10						/* 1 us cycle for timer A. Calculated with 16MHz SMCLK with divider 1 */
#define msTOus  1000
#define sTOus   1000000
#define minTOus 60000000
// on yldse vaja seda?
unsigned long long int timerB_overflow;      /* 71.58 minutes maximum; Will only result in false data when it overflow itself. Could add a short check. */

//struct time runTime = {0,0,0,0};

/* Returns current time ran in microseconds.
 * Max run time ~70minutes - restricted by 4byte type size.
 * Before calling this function disable timer B and afterwards enable it again. */
/*
unsigned long int counter_getCounter(void){
  return (runTime.us + (msTOus*runTime.ms) + (sTOus*runTime.s) + (minTOus*runTime.min));
}
*/
void counter_init(void){
     Timer_B_initContinuousModeParam initContinuousParam = { 0 };
        initContinuousParam.clockSource = TIMER_B_CLOCKSOURCE_SMCLK;                      /* Use SMCLK (faster clock) */
        initContinuousParam.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_8;           /* Input clock = SMCLK / 8 = 2MHz */
        initContinuousParam.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_ENABLE;    /* Enable TAR -> 0 interrupt */
        initContinuousParam.timerClear = TIMER_B_DO_CLEAR;                                /* Clear TAR & clock divider */
        initContinuousParam.startTimer = false;                                           /* Don't start the timer, yet */
    Timer_B_initContinuousMode( TIMER_B0_BASE, &initContinuousParam );                    /* Set up Timer B0 */

    //*************************************************************************
    //  Clear/enable interrupt flags and start timer
    //*************************************************************************
    Timer_B_clearTimerInterrupt( TIMER_B0_BASE );                                 /* Clear TB0IFG */
    Timer_B_clearCaptureCompareInterrupt( TIMER_B0_BASE,
        TIMER_B_CAPTURECOMPARE_REGISTER_0                                         /* Clear interrupt pending flag bit */
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


