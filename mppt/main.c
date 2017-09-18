#include <msp430.h> 
#include "hal.h"
#include "clock.h"
#include "timer.h"

/*
 * main.c
 */

void MSP430config(void);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    MSP430config();

    while(1){
		while(!(TA0CCTL0 && CCIFG));		// wait until interrupt is triggered (1 second is passed)
		timerDebugPort ^= timerDebugPin;	// set debug pin
		TA0CCTL0 &= ~CCIFG;					// clear interrupt flag
    }

	return 0;
}

void MSP430config(void){
	clockConfig();
	timerConfig();
}
