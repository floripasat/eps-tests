#include <msp430.h> 
#include "hal.h"
#include "clock.h"
#include "timer.h"
#include "uart.h"
#include "onewire.h"

/*
 * main.c
 */

void MSP430config(void);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    struct batteryMeasurements_t batteryMeasurements;

    MSP430config();

    DS2784Config();

    while(1){
		while(!(TA0CCTL0 && CCIFG));		// wait until interrupt is triggered (1 second is passed)
		timerDebugPort ^= timerDebugPin;	// set debug pin
		TA0CCTL0 &= ~CCIFG;					// clear interrupt flag

		batteryMeasurements.voltage = (DS2784ReadRegister(voltage_MSB_register) << 8) + DS2784ReadRegister(voltage_LSB_register);
		uartTXFloat((batteryMeasurements.voltage >> 5)*batteryVoltageUnit);
		uartTX(",");
    }

	return 0;
}

void MSP430config(void){
	clockConfig();
	timerConfig();
	uartConfig();
}
