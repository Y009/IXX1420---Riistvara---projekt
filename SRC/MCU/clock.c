/*
 * clock.c
 *
 *  Created on: 6. okt 2016
 *      Author: Karl
 *      ToDo:	use UCS_turnOnXT2WithTimeout
 *
 */

//***** Header Files **********************************************************
#include <driverlib.h>
#include "clock.h"

#define LF_CRYSTAL_FREQUENCY_IN_HZ     32768                                    // 32KHz
#define HF_CRYSTAL_FREQUENCY_IN_HZ     4000000                                  // 4MHz

void clkInit(){

    P5SEL |= BIT2 + BIT3; //XT2CLK select
    P2DIR |= BIT2;
    P2SEL |= BIT2;

    //**************************************************************************
    // Configure core voltage level
    //**************************************************************************

	// Set core voltage level to handle 16MHz clock rate
	PMM_setVCore(PMM_CORE_LEVEL_2);

	 UCS_setExternalClockSource(
	            LF_CRYSTAL_FREQUENCY_IN_HZ,                                         // XT1CLK input
	            HF_CRYSTAL_FREQUENCY_IN_HZ                                          // XT2CLK input
	    );

	 UCS_turnOnXT2( UCS_XT2_DRIVE_4MHZ_8MHZ ); //enable XT2CLK

	// Clock init using DCO/FLL
    // Set REFO as the oscillator reference clock for the FLL
    UCS_initClockSignal(
            UCS_FLLREF,                                  // Clock you're configuring
			UCS_XT2CLK_SELECT,                           // Clock source
            UCS_CLOCK_DIVIDER_1                          // Divide down clock source by this much
    );

    // Set MCLK and SMCLK to use the DCO/FLL as their oscillator source (8MHz)
   	// Calculates required FLL settings; Configures FLL and DCO,
   	// and then sets MCLK and SMCLK to use the DCO (with FLL runtime calibration)
       UCS_initFLLSettle(
               MCLK_DESIRED_FREQUENCY,               // MCLK frequency
               MCLK_FLLREF_RATIO                            // Ratio between MCLK and FLL's reference clock source
       );

}




