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

#define TIMETO 0x10						/* 1 us cycle for timer A. Calculated with 16MHz SMCLK with divider 1 */
#define msTOus  1000
#define sTOus   1000000
#define minTOus 60000000

struct time runTime = {0,0,0,0};

/* Returns current time ran in microseconds.
 * Max run time ~70minutes - restricted by 4byte type size.
 * Before calling this function disable timer B and afterwards enable it again. */
unsigned long int counter_getCounter(void){
  return (runTime.us + (msTOus*runTime.ms) + (sTOus*runTime.s) + (minTOus*runTime.min));
}

void counter_init(void){
    Timer_B_initUpModeParam initUpParam = { 0 };
        initUpParam.clockSource = TIMER_B_CLOCKSOURCE_SMCLK;                      /* Use SMCLK (faster clock) */
        initUpParam.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;           /* Input clock = SMCLK / 1 = 16MHz */
        initUpParam.timerPeriod = TIMETO;                                    	  /* 1us */
        initUpParam.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_ENABLE;    /* Enable TAR -> 0 interrupt */
        initUpParam.captureCompareInterruptEnable_CCR0_CCIE =
                TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;                               /* Enable CCR0 compare interrupt */
        initUpParam.timerClear = TIMER_B_DO_CLEAR;                                /* Clear TAR & clock divider */
        initUpParam.startTimer = false;                                           /* Don't start the timer, yet */
    Timer_B_initUpMode( TIMER_B0_BASE, &initUpParam );                            /* Set up Timer B0 */

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
__interrupt void addToTimer (void){					/* Better way to do this? */
	if (runTime.us < 1000){
		runTime.us++;
	}
	else{
		runTime.us = 1;
		if (runTime.ms < 1000){
			runTime.ms++;
		}
		else{
			runTime.ms = 1;
			if (runTime.s < 60){
				runTime.s++;
			}
			else{
				runTime.s = 1;
				runTime.min++;
			}
		}

	}
    Timer_B_clearTimerInterrupt(TIMER_B0_BASE);										/* Clear TB0IFG */
}


