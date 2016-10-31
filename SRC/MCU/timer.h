/*
 * timer.h
 *
 *  Created on: 23. okt 2016
 *      Author: Y009
 */

#ifndef TIMER_H_
#define TIMER_H_

//***** Defines ***************************************************************

#define enTAI() Timer_A_enableInterrupt(TIMER_A0_BASE)			/* Enable interrupts by Timer A */
#define diTAI() Timer_A_disableInterrupt(TIMER_A0_BASE)			/* Disable interrupts by Timer A */


//***** DEFINES ***************************************************************
void timerInit(void);
void setFlag(int x);
int getFlag(void);

#endif /* TIMER_H_ */
