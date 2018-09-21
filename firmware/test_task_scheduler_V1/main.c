/*
 * main.c
 *
 *  Created on: 11/09/2017
 *      Author: Bruno Eiterer
 *              Sara Vega Martínez
 */

#include <msp430.h>
#include <stdio.h>
#include "hal.h"
#include "ADC.h"
#include "clock.h"
#include "timer.h"
#include "onewire.h"
#include "uart.h"
#include "scheduling.h"

/***** Scheduling ******/
volatile unsigned int LCA = 0;  // variable used to determine the load current action

volatile float volt = 0;
volatile float curr = 0;
volatile float previous_power = 0;
volatile int cont = 0;
volatile float mean_voltage = 0;
volatile float mean_current = 0;
volatile float mean_power = 0;
volatile float accumulated_current = 0; //variable used to save the battery accumulated current value

void MSP430config(void);

void main(void){
	WDTCTL = WDTPW | WDTHOLD;

	struct adcChannels_t adcChannels;
	struct batteryMeasurements_t batteryMeasurements;

	MSP430config();

	DS2784Config();

	debugLedDir |= debugLedPin;
	debugLedPort |= debugLedPin;

	//load5VEnablePort |= load5VEnablePin;	// enable 5V load regulator
	//load5VPSPort |= load5VPSPin;			// disable 5V load regulator PS

	EPS3V3PSPort |= EPS3V3PSPin;			// disable 3V3 EPS regulator PS

	load3V3EnablePort &= ~load3V3EnablePin;	// enable 3V3 load regulator
	load3V3PSPort |= load3V3PSPin;			// disable 3V3 load regulator PS

	while(1){
		
		volt = 0;
		curr = 0;
		cont = 0; 
		
		while(cont!=4){		// wait until interrupt is triggered (1 second is passed)
			adcChannels.pXPanelVoltage = adcRead(pXPanelVoltageAdcChannel);
			mean_voltage = (adcChannels.pXPanelVoltage * panelVoltageUnit);
			adcChannels.pXPanelCurrent = adcRead(pXPanelCurrentAdcChannel);
			mean_current = adcChannels.pXPanelCurrent * panelCurrentUnit;
			volt = volt + mean_voltage;
			curr = curr + mean_current;
			cont ++;
			while(!(TA0CCTL0 && CCIFG));
			timerDebugPort ^= timerDebugPin;    // set debug pin
			TA0CCTL0 &= ~CCIFG;
		}
		

		mean_power = (volt * curr)/cont;
		batteryMeasurements.accumulatedCurrent = (DS2784ReadRegister(accumulated_current_MSB_register) << 8) + DS2784ReadRegister(accumulated_current_LSB_register);
		accumulated_current = batteryMeasurements.accumulatedCurrent * batteryAccumulatedCurrentUnit;
		
		//scheduling
			if(curr>0.02){
				if(previous_power<mean_power){			// determine if previous load current action increased or decreased the input power
					LCA = 0;		// set action to decrease load current (increase battery voltage)
				}else{
					if(previous_power>mean_power){
						LCA = 1;		// set action to increase load current (decrease battery voltage)
					}else{
						LCA = 4;
					} //end else
				} // end else
			}else{
				if(accumulated_current>=0.55 && LCA == 2)   //determine if battery voltage is less than 2,6 to turn off the load
					LCA=2;
				if(accumulated_current<=0.56 && LCA == 3)
					LCA=3;
				if(accumulated_current<0.55)
		 			LCA = 3;
				if(accumulated_current>0.56)
		 			LCA = 2;
			} //end else
				
		previous_power = mean_power;		

		switch(LCA){ //load current action
		        case 0: uartTX("Decrease");
		        break;
		        case 1: uartTX("Increase");
		        break;
		        case 2: uartTX("-Eclipse");
		        break;
		        case 3:uartTX("-TurnOff");
		        break;
		        default: uartTX("NULLNULL");
		    }

		adcChannels.VpanelsVoltage = adcRead(VpanelsAdcChannel);
		uartTXFloat(adcChannels.VpanelsVoltage * VpanelsUnit);
		uartTX(",");
		adcChannels.pXPanelVoltage = adcRead(pXPanelVoltageAdcChannel);
		uartTXFloat(adcChannels.pXPanelVoltage * panelVoltageUnit);
		uartTX(",");
		adcChannels.nXPanelVoltage = adcRead(nXPanelVoltageAdcChannel);
		uartTXFloat(adcChannels.nXPanelVoltage * panelVoltageUnit);
		uartTX(",");
		adcChannels.pYPanelVoltage = adcRead(pYPanelVoltageAdcChannel);
		uartTXFloat(adcChannels.pYPanelVoltage * panelVoltageUnit);
		uartTX(",");
		adcChannels.nYPanelVoltage = adcRead(nYPanelVoltageAdcChannel);
		uartTXFloat(adcChannels.nYPanelVoltage * panelVoltageUnit);
		uartTX(",");
		adcChannels.VchargeVoltage = adcRead(VchargeAdcChannel);
		uartTXFloat(adcChannels.VchargeVoltage * VchargeUnit);
		uartTX(",");
        adcChannels.pXPanelCurrent = adcRead(pXPanelCurrentAdcChannel);
		uartTXFloat(adcChannels.pXPanelCurrent * panelCurrentUnit);
		uartTX(",");
		adcChannels.nXPanelCurrent = adcRead(nXPanelCurrentAdcChannel);
		uartTXFloat(adcChannels.nXPanelCurrent * panelCurrentUnit);
		uartTX(",");
		adcChannels.pYPanelCurrent = adcRead(pYPanelCurrentAdcChannel);
		uartTXFloat(adcChannels.pYPanelCurrent * panelCurrentUnit);
		uartTX(",");
		adcChannels.nYPanelCurrent = adcRead(nYPanelCurrentAdcChannel);
		uartTXFloat(adcChannels.nYPanelCurrent * panelCurrentUnit);
		uartTX(",");
		adcChannels.pZPanelCurrent = adcRead(pZPanelCurrentAdcChannel);
		uartTXFloat(adcChannels.pZPanelCurrent * panelCurrentUnit);
		uartTX(",");
		adcChannels.loadCurrent = adcRead(loadCurrentAdcChannel);
		uartTXFloat(adcChannels.loadCurrent * loadCurrentUnit);
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

		//eclipse
		if((adcChannels.nXPanelCurrent + adcChannels.nYPanelCurrent + adcChannels.pXPanelCurrent + adcChannels.pYPanelCurrent + adcChannels.pZPanelCurrent)*panelCurrentUnit < 0.03
				& (adcChannels.nXPanelVoltage + adcChannels.nYPanelVoltage + adcChannels.pXPanelVoltage + adcChannels.pYPanelVoltage)*panelVoltageUnit < 4){
		//	load3V3EnablePort |= load3V3EnablePin;
		    LCA = 2;
		} // end if
	}
}

void MSP430config(void){
	clockConfig();
	timerConfig();
	adcConfig();
	uartConfig();

	//load5VEnableDir |= load5VEnablePin;		// set 5V load regulator enable as output
	//load5VPSDir |= load5VPSPin;				// set 5V load regulator PS as output

	EPS3V3PSDir |= EPS3V3PSPin;				// set 3V3 EPS regulator PS as output

	load3V3EnableDir |= load3V3EnablePin;	// set 3V3 load regulator enable as output
	load3V3PSDir |= load3V3PSPin;			// set 3V3 load regulator PS as output
}
