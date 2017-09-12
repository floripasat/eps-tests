/*
 * main.c
 *
 *  Created on: 11/09/2017
 *      Author: Bruno Eiterer
 */

#include <msp430.h>
#include "hal.h"
#include "ADC.h"

void MSP430config(void);

void main(void){
	WDTCTL = WDTPW | WDTHOLD;

	struct adcChannels_t adcChannels;

	MSP430config();

	debugLedDir |= debugLedPin;
	debugLedPort |= debugLedPin;

	while(1){
		adcChannels.VpanelsVoltage = adcRead(VpanelsAdcChannel);
	}
}

void MSP430config(void){
	adcConfig();
}
