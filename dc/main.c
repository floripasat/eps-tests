/*
 * main.c
 *
 *  Created on: 11/09/2017
 *      Author: Bruno Eiterer
 */

#include <msp430.h>
#include "hal.h"

void main(void){
	WDTCTL = WDTPW | WDTHOLD;

	debugLedDir |= debugLedPin;
	debugLedPort |= debugLedPin;

	while(1);
}
