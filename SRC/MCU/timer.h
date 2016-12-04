/*
 * timer.h
 *
 *  Created on: 23. okt 2016
 *      Author: Y009
 */

#ifndef TIMER_H_
#define TIMER_H_

//***** Defines ***************************************************************

#define timer_enTAI() Timer_A_enableInterrupt(TIMER_A0_BASE)			/* Enable interrupts by Timer A */ // _enable_interrupts()
#define timer_diTAI() Timer_A_disableInterrupt(TIMER_A0_BASE)			/* Disable interrupts by Timer A */ // _disable_interrupts()

void timer_init(void);
int timer_getCounter(void);
void timer_checkFlag();

#endif /* TIMER_H_ */
