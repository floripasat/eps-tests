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
		adcChannels.pXPanelVoltage = adcRead(pXPanelVoltageAdcChannel);
		adcChannels.nXPanelVoltage = adcRead(nXPanelVoltageAdcChannel);
		adcChannels.pYPanelVoltage = adcRead(pYPanelVoltageAdcChannel);
		adcChannels.nYPanelVoltage = adcRead(nYPanelVoltageAdcChannel);
		adcChannels.pZPanelVoltage = adcRead(pZPanelVoltageAdcChannel);

		adcChannels.loadCurrent = adcRead(loadCurrentAdcChannel);
		adcChannels.pXPanelCurrent = adcRead(pXPanelCurrentAdcChannel);
		adcChannels.nXPanelCurrent = adcRead(nXPanelCurrentAdcChannel);
	}
}

void MSP430config(void){
	adcConfig();
}
