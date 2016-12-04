/*
 * counter.h
 *
 *  Created on: 31. okt 2016
 *      Author: Y009
 */

#ifndef COUNTER_H_
#define COUNTER_H_

//***** Defines ***************************************************************

#define counter_enTBI() Timer_B_enableInterrupt(TIMER_B0_BASE)			/* Enable interrupts by Timer B */
#define counter_diTBI() Timer_B_disableInterrupt(TIMER_B0_BASE)			/* Disable interrupts by Timer B */

unsigned long long int counter_getOverflow(void);
void counter_init(void);

#endif /* COUNTER_H_ */
