/*
 * ultraS.h
 *
 *  Created on: 7. nov 2016
 *      Author: Y009
 */

/** \file ultraS.h
**	\brief Documentation for the ultrasonic module's header.
**
**  Global enumeration defines.
**/

#ifndef ULTRAS_H_
#define ULTRAS_H_

//***** Defines ***************************************************************

typedef enum{                                       	/* States for ultrasonic module. */
	US_OK,                                            	/**< Work done. */
	US_WORKING,                                        	/**< Working. */
	US_ERROR,                                          	/**< Error while working. */
	US_IDLE,                                           	/**< Waiting for work. */
	US_NUMBER_OF_TYPES
}status;

typedef enum{                                           /* States for data preparation. */
	US_DATA_FALSEMIN,                                   /**< Under the minimum 2cm distance. */
	US_DATA_FALSEMAX,                                   /**< Over the maximum 4m distance. */
	US_DATA_TRUE,                                       /**< Data should be good and is ready to be read. */
	US_DATA_TRUE2,                                      /**< Time has been read from counter and is ready for processing. */
	US_DATA_FALSE,                                      /**< Initial status indicating the need to read data, so we don't read before doing any work. */
	US_DATA_READ,                                       /**< If Read, then application has read the data. */
	US_DATA_NUMBER_OF_TYPES
}dataStatus;

/** \cond IGNORE*/
void ultraS_init();
void ultraS_sendSignal();
void ultraS_prepInfo();
void ultraS_cyclic();
unsigned int ultraS_getValidStatus();
void ultraS_setValidStatus(enum status validStatus);
unsigned int ultraS_getDistance();
int ultraS_getDataStatus();
void ultraS_setDataStatus(enum dataStatus validStatus);
/** \endcond IGNORE*/

#endif /* ULTRAS_H_ */
