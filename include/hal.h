/*
 * hal.h
 *
 *  Created on: 11/09/2017
 *      Author: Bruno Eiterer
 */

#ifndef HAL_H_
#define HAL_H_

/**** ADC Channels ****/
#define	VpanelsAdcChannel			0x01
#define	pXPanelVoltageAdcChannel	0x0C
#define	nXPanelVoltageAdcChannel	0x04
#define	pYPanelVoltageAdcChannel	0x0D
#define	nYPanelVoltageAdcChannel	0x0E
#define	pZPanelVoltageAdcChannel	0x03

#define loadCurrentAdcChannel		0x00
#define pXPanelCurrentAdcChannel	0x07
#define nXPanelCurrentAdcChannel	0x05
#define pYPanelCurrentAdcChannel	0x0F
#define nYPanelCurrentAdcChannel	0x06
#define pZPanelCurrentAdcChannel	0x02

/**** ADC Units ****/
#define VpanelsUnit	0.00106133355034722				// (2.5/2^12)*(180+133)/180
#define panelVoltageUnit	0.00106133355034722		// (2.5/2^12)*(180+133)/180

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

#endif /* HAL_H_ */
