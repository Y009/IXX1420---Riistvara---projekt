/*
 * counter.h
 *
 *  Created on: 31. okt 2016
 *      Author: Y009
 */

#ifndef COUNTER_H_
#define COUNTER_H_

//***** Defines ***************************************************************

#define counter_enTBI(timerBase)			Timer_B_enableInterrupt(timerBase)			/* Enable interrupts by Timer B */
#define counter_diTBI(timerBase)			Timer_B_disableInterrupt(timerBase)			/* Disable interrupts by Timer B */
#define counter_getCountValue(timerBase)	Timer_B_getCounterValue(timerBase);

#define counter_timerBase					TIMER_B0_BASE

unsigned long long int counter_getOverflow(void);
void counter_init(void);

#endif /* COUNTER_H_ */
