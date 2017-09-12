/*
 * hal.h
 *
 *  Created on: 11/09/2017
 *      Author: Bruno Eiterer
 */

#ifndef HAL_H_
#define HAL_H_

/**** ADC ****/
#define	VpanelsAdcChannel	0x01
#define	pXPanelVoltageAdcChannel	0x0C
#define	nXPanelVoltageAdcChannel	0x04
#define	pYPanelVoltageAdcChannel	0x0D
#define	nYPanelVoltageAdcChannel	0x0E
#define	pZPanelVoltageAdcChannel	0x03

#define loadCurrentAdcChannel	0x00
#define pXPanelCurrentAdcChannel	0x07
#define nXPanelCurrentAdcChannel	0x05
#define pYPanelCurrentAdcChannel	0x0F
#define nYPanelCurrentAdcChannel	0x06
#define pZPanelCurrentAdcChannel	0x02

/**** Debugging LED ****/
#define debugLedDir	P5DIR
#define debugLedPort P5OUT
#define debugLedPin BIT4

/**** Timer Debug ****/
#define timerDebugDir	P2DIR
#define timerDebugPort	P2OUT
#define timerDebugPin	BIT0

#endif /* HAL_H_ */
