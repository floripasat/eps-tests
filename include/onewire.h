/*
 * eps_onewire_test.h
 *
 *  Created on: 12/04/2016
 *      Author: Bruno Eiterer
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>
#include "hal.h"

#define clock 8000000

/**** UNITS ****/

#define	rsense			0.02
#define voltage_unit	0.004883
#define current_unit	0.0000015625/rsense
#define accumulated_current_unit	6.25*0.000001/rsense

/**** COMMANDS ****/
#define writeRegisterCommand 0x6C
#define readRegisterCommand 0x69
#define skipNetAddress 0xCC
#define copyData 0x48

struct batteryMeasurements_t{
	uint16_t voltage;
	uint16_t current;
	uint16_t averageCurrent;
	uint16_t temperature;
	uint16_t accumulatedCurrent;
	uint8_t statusRegister;
	uint8_t protectionRegister;
};

void DS2784Config(void);

void outp(int bit);
int inp(void);
int OneWireReset(void);
void OneWireWrite(int bit);
int OneWireReadBit(void);
void OWWriteByte(int data);
uint8_t OWReadByte(void);
uint8_t DS2784ReadRegister(uint8_t register_address);

#endif /* EPS_ONEWIRE_TEST_H_ */


