/*
 * ultraS.h
 *
 *  Created on: 7. nov 2016
 *      Author: Y009
 */

#ifndef ULTRAS_H_
#define ULTRAS_H_

//***** Defines ***************************************************************

#define True 	 2									/* 3 states for data preparation. */
#define FalseMin 1
#define FalseMax 0

void ultraS_init();
void ultraS_sendSignal();
void ultraS_prepInfo();
char ultraS_getValidStatus();
unsigned int ultraS_getDistance();

#endif /* ULTRAS_H_ */
