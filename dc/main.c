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
#include "onewire.h"
#include "uart.h"

void MSP430config(void);

void main(void){
	WDTCTL = WDTPW | WDTHOLD;

	struct adcChannels_t adcChannels;
	struct batteryMeasurements_t batteryMeasurements;

	MSP430config();

	DS2784Config();

	debugLedDir |= debugLedPin;
	debugLedPort |= debugLedPin;

	load5VEnablePort |= load5VEnablePin;	// enable 5V load regulator
	load5VPSPort |= load5VPSPin;			// disable 5V load regulator PS

	EPS3V3PSPort |= EPS3V3PSPin;			// disable 3V3 EPS regulator PS

	load3V3EnablePort |= load3V3EnablePin;	// enable 3V3 load regulator
	load3V3PSPort |= load3V3PSPin;			// disable 3V3 load regulator PS

	while(1){
		while(!(TA0CCTL0 && CCIFG));		// wait until interrupt is triggered (1 second is passed)
		timerDebugPort ^= timerDebugPin;	// set debug pin
		TA0CCTL0 &= ~CCIFG;					// clear interrupt flag

		adcChannels.VpanelsVoltage = adcRead(VpanelsAdcChannel);
		uartTXFloat(adcChannels.VpanelsVoltage*VpanelsUnit);
		uartTX(",");
		adcChannels.pXPanelVoltage = adcRead(pXPanelVoltageAdcChannel);
		uartTXFloat(adcChannels.pXPanelVoltage*panelVoltageUnit);
		uartTX(",");
		adcChannels.nXPanelVoltage = adcRead(nXPanelVoltageAdcChannel);
		uartTXFloat(adcChannels.nXPanelVoltage*panelVoltageUnit);
		uartTX(",");
		adcChannels.pYPanelVoltage = adcRead(pYPanelVoltageAdcChannel);
		uartTXFloat(adcChannels.pYPanelVoltage*panelVoltageUnit);
		uartTX(",");
		adcChannels.nYPanelVoltage = adcRead(nYPanelVoltageAdcChannel);
		adcChannels.pZPanelVoltage = adcRead(pZPanelVoltageAdcChannel);

		adcChannels.loadCurrent = adcRead(loadCurrentAdcChannel);
		adcChannels.pXPanelCurrent = adcRead(pXPanelCurrentAdcChannel);
		adcChannels.nXPanelCurrent = adcRead(nXPanelCurrentAdcChannel);
		adcChannels.pYPanelCurrent = adcRead(pYPanelCurrentAdcChannel);
		adcChannels.nYPanelCurrent = adcRead(nYPanelCurrentAdcChannel);
		adcChannels.pZPanelCurrent = adcRead(pZPanelCurrentAdcChannel);

		batteryMeasurements.voltage = (DS2784ReadRegister(voltage_MSB_register) << 8) + DS2784ReadRegister(voltage_LSB_register);
		batteryMeasurements.current = (DS2784ReadRegister(current_MSB_register) << 8) + DS2784ReadRegister(current_LSB_register);
		batteryMeasurements.averageCurrent = (DS2784ReadRegister(average_current_MSB_register) << 8) + DS2784ReadRegister(average_current_LSB_register);
		batteryMeasurements.temperature = (DS2784ReadRegister(temperature_MSB_register) << 8) + DS2784ReadRegister(temperature_LSB_register);
		batteryMeasurements.protectionRegister = DS2784ReadRegister(protection_register);
		batteryMeasurements.statusRegister = DS2784ReadRegister(status_register);
		batteryMeasurements.accumulatedCurrent = (DS2784ReadRegister(accumulated_current_MSB_register) << 8) + DS2784ReadRegister(accumulated_current_LSB_register);
	}
}

void MSP430config(void){
	clockConfig();
	timerConfig();
	adcConfig();
	uartConfig();

	load5VEnableDir |= load5VEnablePin;		// set 5V load regulator enable as output
	load5VPSDir |= load5VPSPin;				// set 5V load regulator PS as output

	EPS3V3PSDir |= EPS3V3PSPin;				// set 3V3 EPS regulator PS as output

	load3V3EnableDir |= load3V3EnablePin;	// set 3V3 load regulator enable as output
	load3V3PSDir |= load3V3PSPin;			// set 3V3 load regulator PS as output
}
