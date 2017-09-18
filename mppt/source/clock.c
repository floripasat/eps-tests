/*
 * clock.c
 *
 *  Created on: 06/04/2017
 *      Author: Bruno Eiterer
 */

#include <msp430.h>
#include "clock.h"

void clockConfig(void){

	P7SEL |= BIT2 + BIT3; 	// select XT2 function
	UCSCTL6 &= ~XT2OFF;	  	// Enable XT2
	UCSCTL6 |= XCAP_3;		// Internal load cap

	UCSCTL4 |= SELA__XT2CLK + SELS__XT2CLK + SELM__XT2CLK;  // SMCLK = MCLK = ACLK = XT2
	UCSCTL5 |= DIVM__4 + DIVS__32 + DIVA__2;				// XT2Clock = 32 MHz, MCLK = 8 MHz, SMCLK = 1 MHz, ACLK = 16 MHz

	do {
		UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);  // Clear XT2,XT1,DCO fault flags
		SFRIFG1 &= ~OFIFG;	// Clear fault flags
	} while (SFRIFG1 & OFIFG);
}
