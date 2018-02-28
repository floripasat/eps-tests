/*
 * hal.h
 *
 *  Created on: 11/09/2017
 *      Author: Bruno Eiterer
 */

#ifndef HAL_H_
#define HAL_H_

/**** ADC Channels ****/
#define	VpanelsAdcChannel			0x0F
#define	pXPanelVoltageAdcChannel	0x01
#define	nXPanelVoltageAdcChannel	0x04
#define	pYPanelVoltageAdcChannel	0x06
#define	nYPanelVoltageAdcChannel	0x0D
#define	VchargeAdcChannel			0x05

#define loadCurrentAdcChannel		0x02
#define pXPanelCurrentAdcChannel	0x00
#define nXPanelCurrentAdcChannel	0x03
#define pYPanelCurrentAdcChannel	0x07
#define nYPanelCurrentAdcChannel	0x0E
#define pZPanelCurrentAdcChannel	0x0C

/**** ADC Units ****/
#define VpanelsUnit	0.001220703125					// (2.5/2^12)*(100+100)/100
#define panelVoltageUnit	0.001220703125			// (2.5/2^12)*(100+100)/100
#define panelCurrentUnit	0.000048828125			// (2.5/2^12)/(0.05*0.025*10000)
#define loadCurrentUnit		0.00048828125			// (2.5/2^12)/(0.05*0.025*1000)
#define VchargeUnit			0.00106926251174812		// (2.5/2^12)*(100+133)/133

/**** Regulators Enable and Power Save ****/
#define load5VEnableDir		P1DIR
#define load5VEnablePort	P1OUT
#define load5VEnablePin		BIT3
#define load5VPSDir			P1DIR
#define load5VPSPort		P1OUT
#define load5VPSPin			BIT5

#define EPS3V3PSDir			P1DIR
#define EPS3V3PSPort		P1OUT
#define EPS3V3PSPin			BIT4

#define load3V3EnableDir	P1DIR
#define load3V3EnablePort	P1OUT
#define load3V3EnablePin	BIT2
#define load3V3PSDir		P1DIR
#define load3V3PSPort		P1OUT
#define load3V3PSPin		BIT7

/**** Debugging LED ****/
#define debugLedDir	P5DIR
#define debugLedPort P5OUT
#define debugLedPin BIT4

/**** Timer Debug ****/
#define timerDebugDir	P2DIR
#define timerDebugPort	P2OUT
#define timerDebugPin	BIT0

/**** DS2784 Pins ****/

#define P_WireOUT P9OUT
#define P_WireIN P9IN
#define DIR_P_Wire P9DIR
#define BitWire	BIT1

/**** DS2784 Registers ****/

#define protection_register 0x00
#define protector_threshold_register 0x7F
#define status_register 0x01
#define control_register 0x60
#define accumulated_current_MSB_register 0x10
#define accumulated_current_LSB_register 0x11
#define temperature_MSB_register 0x0A
#define temperature_LSB_register 0x0B
#define average_current_MSB_register 0x08
#define average_current_LSB_register 0x09
#define current_gain_MSB_register 0x78
#define current_gain_LSB_register  0x79
#define current_MSB_register 0x0E
#define current_LSB_register 0x0F
#define voltage_MSB_register 0x0C
#define voltage_LSB_register 0x0D

/**** DS2784 Units ****/

#define	rsense							0.015
#define batteryVoltageUnit				0.004883
#define batteryCurrentUnit				0.0000015625/rsense
#define batteryAccumulatedCurrentUnit	6.25*0.000001/rsense
#define batteryMonitorTemperatureUnit	0.125

#endif /* HAL_H_ */
