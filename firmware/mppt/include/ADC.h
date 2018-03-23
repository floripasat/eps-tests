/*
 * eps_ADC.h
 *
 *  Created on: 16/11/2016
 *      Author: Bruno Eiterer
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void adcConfig(void);

int16_t adcRead(uint8_t channel);

struct adcChannels_t{
	/**** Currents ****/
	uint16_t loadCurrent;		// load current
	uint16_t pXPanelCurrent;	// +X panel current
	uint16_t nXPanelCurrent;	// -X panel current
	uint16_t pYPanelCurrent;	// +Y panel current
	uint16_t nYPanelCurrent;	// -Y panel current
	uint16_t pZPanelCurrent;	// +Z panel current

	/**** Voltages ****/
	uint16_t VpanelsVoltage;	// Vpanels voltage
	uint16_t pXPanelVoltage;	// +X panel voltage
	uint16_t nXPanelVoltage;	// -X panel voltage
	uint16_t pYPanelVoltage;	// +Y panel voltage
	uint16_t nYPanelVoltage;	// -Y panel voltage
	uint16_t VchargeVoltage;	// Vcharge voltage
};

#endif /* ADC_H_ */
