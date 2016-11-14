/*
 * counter.h
 *
 *  Created on: 31. okt 2016
 *      Author: Y009
 */

#ifndef COUNTER_H_
#define COUNTER_H_

//***** Defines ***************************************************************

#define timer_enTBI() Timer_B_enableInterrupt(TIMER_B0_BASE)			/* Enable interrupts by Timer B */
#define timer_diTBI() Timer_B_disableInterrupt(TIMER_B0_BASE)			/* Disable interrupts by Timer B */

struct time {
	unsigned short int us;
	unsigned short int ms;
	unsigned short int s;
	unsigned short int min;
};

unsigned long int counter_getCounter(void);
void counter_init(void);

#endif /* COUNTER_H_ */
