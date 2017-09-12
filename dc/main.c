/*
 * main.c
 *
 *  Created on: 11/09/2017
 *      Author: Bruno Eiterer
 */

#include <msp430.h>
#include "hal.h"
#include "ADC.h"
#include "clock.h"
#include "timer.h"

void MSP430config(void);

void main(void){
	WDTCTL = WDTPW | WDTHOLD;

	struct adcChannels_t adcChannels;

	MSP430config();

	debugLedDir |= debugLedPin;
	debugLedPort |= debugLedPin;

	while(1){
		while(!(TA0CCTL0 && CCIFG));			// wait until interrupt is triggered (1 second is passed)
		timerDebugPort ^= timerDebugPin;	// set debug pin
		TA0CCTL0 &= ~CCIFG;					// clear interrupt flag

		adcChannels.VpanelsVoltage = adcRead(VpanelsAdcChannel);
		adcChannels.pXPanelVoltage = adcRead(pXPanelVoltageAdcChannel);
		adcChannels.nXPanelVoltage = adcRead(nXPanelVoltageAdcChannel);
		adcChannels.pYPanelVoltage = adcRead(pYPanelVoltageAdcChannel);
		adcChannels.nYPanelVoltage = adcRead(nYPanelVoltageAdcChannel);
		adcChannels.pZPanelVoltage = adcRead(pZPanelVoltageAdcChannel);

		adcChannels.loadCurrent = adcRead(loadCurrentAdcChannel);
		adcChannels.pXPanelCurrent = adcRead(pXPanelCurrentAdcChannel);
		adcChannels.nXPanelCurrent = adcRead(nXPanelCurrentAdcChannel);
		adcChannels.pYPanelCurrent = adcRead(pYPanelCurrentAdcChannel);
		adcChannels.nYPanelCurrent = adcRead(nYPanelCurrentAdcChannel);
		adcChannels.pZPanelCurrent = adcRead(pZPanelCurrentAdcChannel);
	}
}

void MSP430config(void){
	clockConfig();
	timerConfig();
	adcConfig();
}
