#include <msp430.h> 
#include "hal.h"

/*
 * main.c
 */

void MSP430config(void);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    MSP430config();

    while(1);

	return 0;
}

void MSP430config(void){

}
