/*
 * main.c
 *
 *  Created on: 11/09/2017
 *      Author: Bruno Eiterer
 */

#include <msp430.h>
#include <stdio.h>
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
		uartTXFloat(adcChannels.nYPanelVoltage*panelVoltageUnit);
		uartTX(",");
		adcChannels.pZPanelVoltage = adcRead(pZPanelVoltageAdcChannel);
		uartTXFloat(adcChannels.pZPanelVoltage*panelVoltageUnit);
		uartTX(",");

		adcChannels.pXPanelCurrent = adcRead(pXPanelCurrentAdcChannel);
		uartTXFloat(adcChannels.pXPanelCurrent*panelCurrentUnit);
		uartTX(",");
		adcChannels.nXPanelCurrent = adcRead(nXPanelCurrentAdcChannel);
		uartTXFloat(adcChannels.nXPanelCurrent*panelCurrentUnit);
		uartTX(",");
		adcChannels.pYPanelCurrent = adcRead(pYPanelCurrentAdcChannel);
		uartTXFloat(adcChannels.pYPanelCurrent*panelCurrentUnit);
		uartTX(",");
		adcChannels.nYPanelCurrent = adcRead(nYPanelCurrentAdcChannel);
		uartTXFloat(adcChannels.nYPanelCurrent*panelCurrentUnit);
		uartTX(",");
		adcChannels.pZPanelCurrent = adcRead(pZPanelCurrentAdcChannel);
		uartTXFloat(adcChannels.pZPanelCurrent*panelCurrentUnit);
		uartTX(",");
		adcChannels.loadCurrent = adcRead(loadCurrentAdcChannel);
		uartTXFloat(adcChannels.loadCurrent*loadCurrentUnit);
		uartTX(",");

		uint8_t auxString[4];

		batteryMeasurements.voltage = (DS2784ReadRegister(voltage_MSB_register) << 8) + DS2784ReadRegister(voltage_LSB_register);
		uartTXFloat((batteryMeasurements.voltage >> 5)*batteryVoltageUnit);
		uartTX(",");
		batteryMeasurements.current = (DS2784ReadRegister(current_MSB_register) << 8) + DS2784ReadRegister(current_LSB_register);
		uartTXFloat(batteryMeasurements.current*batteryCurrentUnit);
		uartTX(",");
		batteryMeasurements.averageCurrent = (DS2784ReadRegister(average_current_MSB_register) << 8) + DS2784ReadRegister(average_current_LSB_register);
		uartTXFloat(batteryMeasurements.averageCurrent*batteryCurrentUnit);
		uartTX(",");
		batteryMeasurements.temperature = (DS2784ReadRegister(temperature_MSB_register) << 8) + DS2784ReadRegister(temperature_LSB_register);
		uartTXFloat((batteryMeasurements.temperature >> 5)*batteryMonitorTemperatureUnit);
		uartTX(",");
		batteryMeasurements.protectionRegister = DS2784ReadRegister(protection_register);
		sprintf(auxString, "%#04X", batteryMeasurements.protectionRegister);
		uartTX(auxString);
		uartTX(",");
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
