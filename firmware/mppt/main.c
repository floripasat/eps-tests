#include <msp430.h> 
#include <stdio.h>
#include "hal.h"
#include "clock.h"
#include "timer.h"
#include "uart.h"
#include "onewire.h"
#include "adc.h"

/*
 * main.c
 */

void MSP430config(void);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    struct batteryMeasurements_t batteryMeasurements;
    struct adcChannels_t adcChannels;

    MSP430config();

    DS2784Config();

	debugLedDir |= debugLedPin;
	debugLedPort |= debugLedPin;

	load5VEnablePort |= load5VEnablePin;	// enable 5V load regulator
	load5VPSPort |= load5VPSPin;			// disable 5V load regulator PS

	EPS3V3PSPort |= EPS3V3PSPin;			// disable 3V3 EPS regulator PS

	load3V3EnablePort |= load3V3EnablePin;	// enable 3V3 load regulator
	load3V3PSPort |= load3V3PSPin;			// disable 3V3 load regulator PS

 	pXmpptShutdownPort |= pXmpptShutdownPin;		// enable +X mppt
 	nXmpptShutdownPort |= nXmpptShutdownPin;		// enable -X mppt
 	pYmpptShutdownPort |= pYmpptShutdownPin;		// enable +Y mppt
 	nYmpptShutdownPort |= nYmpptShutdownPin;		// enable -Y mppt
 	pZmpptShutdownPort |= pZmpptShutdownPin;		// enable +Z mppt
 	nZmpptShutdownPort |= nZmpptShutdownPin;		// enable -Z mppt

    while(1){
		while(!(TA0CCTL0 && CCIFG));		// wait until interrupt is triggered (1 second is passed)
		timerDebugPort ^= timerDebugPin;	// set debug pin
		TA0CCTL0 &= ~CCIFG;					// clear interrupt flag

		static uint8_t i = 0;

		adcChannels.VpanelsVoltage += adcRead(VpanelsAdcChannel);
		adcChannels.pXPanelVoltage += adcRead(pXPanelVoltageAdcChannel);
		adcChannels.nXPanelVoltage += adcRead(nXPanelVoltageAdcChannel);
		adcChannels.pYPanelVoltage += adcRead(pYPanelVoltageAdcChannel);
		adcChannels.nYPanelVoltage += adcRead(nYPanelVoltageAdcChannel);
		adcChannels.VchargeVoltage += adcRead(VchargeAdcChannel);
		adcChannels.pXPanelCurrent += adcRead(pXPanelCurrentAdcChannel);
		adcChannels.nXPanelCurrent += adcRead(nXPanelCurrentAdcChannel);
		adcChannels.pYPanelCurrent += adcRead(pYPanelCurrentAdcChannel);
		adcChannels.nYPanelCurrent += adcRead(nYPanelCurrentAdcChannel);
		adcChannels.pZPanelCurrent += adcRead(pZPanelCurrentAdcChannel);
		adcChannels.loadCurrent += adcRead(loadCurrentAdcChannel);

		i++;

		if(i == 9){

			i = 0;

			adcChannels.VpanelsVoltage /= 10;
			uartTXFloat(adcChannels.VpanelsVoltage*VpanelsUnit);
			uartTX(",");
			adcChannels.pXPanelVoltage /= 10;
			uartTXFloat(adcChannels.pXPanelVoltage*panelVoltageUnit);
			uartTX(",");
			adcChannels.nXPanelVoltage /= 10;
			uartTXFloat(adcChannels.nXPanelVoltage*panelVoltageUnit);
			uartTX(",");
			adcChannels.pYPanelVoltage /= 10;
			uartTXFloat(adcChannels.pYPanelVoltage*panelVoltageUnit);
			uartTX(",");
			adcChannels.nYPanelVoltage /= 10;
			uartTXFloat(adcChannels.nYPanelVoltage*panelVoltageUnit);
			uartTX(",");
			adcChannels.VchargeVoltage /= 10;
			uartTXFloat(adcChannels.VchargeVoltage*VchargeUnit);
			uartTX(",");
			adcChannels.pXPanelCurrent /= 10;
			uartTXFloat(adcChannels.pXPanelCurrent*panelCurrentUnit);
			uartTX(",");
			adcChannels.nXPanelCurrent /= 10;
			uartTXFloat(adcChannels.nXPanelCurrent*panelCurrentUnit);
			uartTX(",");
			adcChannels.pYPanelCurrent /= 10;
			uartTXFloat(adcChannels.pYPanelCurrent*panelCurrentUnit);
			uartTX(",");
			adcChannels.nYPanelCurrent /= 10;
			uartTXFloat(adcChannels.nYPanelCurrent*panelCurrentUnit);
			uartTX(",");
			adcChannels.pZPanelCurrent /= 10;
			uartTXFloat(adcChannels.pZPanelCurrent*panelCurrentUnit);
			uartTX(",");
			adcChannels.loadCurrent /= 10;
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
			sprintf(auxString, "%#04X", batteryMeasurements.statusRegister);
			uartTX(auxString);
			uartTX(",");
			batteryMeasurements.accumulatedCurrent = (DS2784ReadRegister(accumulated_current_MSB_register) << 8) + DS2784ReadRegister(accumulated_current_LSB_register);
			uartTXFloat(batteryMeasurements.accumulatedCurrent*batteryAccumulatedCurrentUnit);
			uartTX("\r\n");

			if((adcChannels.nXPanelCurrent + adcChannels.nYPanelCurrent + adcChannels.pXPanelCurrent + adcChannels.pYPanelCurrent + adcChannels.pZPanelCurrent)*panelCurrentUnit < 0.03){
				load3V3EnablePort |= load3V3EnablePin;
			}else{
				load3V3EnablePort &= ~load3V3EnablePin;
			}

		}
    }

	return 0;
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

 	pXmpptShutdownDir |= pXmpptShutdownPin;		// set +X mppt shutdown as output
 	nXmpptShutdownDir |= nXmpptShutdownPin;		// set -X mppt shutdown as output
 	pYmpptShutdownDir |= pYmpptShutdownPin;		// set +Y mppt shutdown as output
 	nYmpptShutdownDir |= nYmpptShutdownPin;		// set -Y mppt shutdown as output
 	pZmpptShutdownDir |= pZmpptShutdownPin;		// set +Z mppt shutdown as output
 	nZmpptShutdownDir |= nZmpptShutdownPin;		// set -Z mppt shutdown as output
}
