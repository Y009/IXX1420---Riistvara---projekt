/*
 * clock.h
 *
 *  Created on: 6. okt 2016
 *      Author: Karl
 */

/** \file clock.h
**	\brief Documentation for the clock's header.
**
**  Global macro defines.
**/

#ifndef CLOCK_H_
#define CLOCK_H_


//***** Defines ***************************************************************
#define MCLK_DESIRED_FREQUENCY  16000                /**< desired clock frequency in Khz */                    
#define MCLK_FLLREF_RATIO              MCLK_DESIRED_FREQUENCY / 4000	/**< Ratio of desired clock frequency to refrence clock frequency */


/** \cond IGNORE*/
//***** Prototypes *************************************************
void clkInit(void);
/** \endcond IGNORE*/
#endif /* CLOCK_H_ */
