/*
 * ultraS.h
 *
 *  Created on: 7. nov 2016
 *      Author: Y009
 */

#ifndef ULTRAS_H_
#define ULTRAS_H_

//***** Defines ***************************************************************
/*
#define True 	 2
#define FalseMin 1
#define FalseMax 0
*/
enum status{                                        /* States for ultrasonic module. */
    ok,                                             /* Ready for work. */
    busy,                                           /* Working. */
    error                                           /* Error while working. */
};
enum dataStatus{                                    /* States for data preparation. */
    FalseMin,                                       /* Under the minimum 2cm distance. */
    FalseMax,                                       /* Over the maximum 4m distance. */
    True,                                           /* Data should be good and is ready to be read. */
    True2,                                          /* Time has been read from counter and is ready for processing. */
    False,                                          /* Initial status indicating the need to read data, so we don't read before doing any work. */
    Read                                            /* If Read, then application has read the data. */
};

void ultraS_init();
void ultraS_sendSignal();
void ultraS_prepInfo();
short ultraS_getValidStatus();
void ultraS_setValidStatus(validStatus);
unsigned int ultraS_getDistance();
void ultraS_setDistance(x);
enum usStatus ultraS_getStatus();

#endif /* ULTRAS_H_ */
