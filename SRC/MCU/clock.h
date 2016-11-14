/*
 * clock.h
 *
 *  Created on: 6. okt 2016
 *      Author: Karl
 */

#ifndef CLOCK_H_
#define CLOCK_H_

//***** Defines ***************************************************************
#define MCLK_DESIRED_FREQUENCY  16000                                    // 16MHz
#define MCLK_FLLREF_RATIO              MCLK_DESIRED_FREQUENCY / 4000//(UCS_REFOCLK_FREQUENCY/1000)

//***** Prototypes *************************************************
void clkInit(void);

#endif /* CLOCK_H_ */
