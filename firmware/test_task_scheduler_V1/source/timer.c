#include "timer.h"

void timerConfig(void){

	timerDebugDir |= timerDebugPin;					// P1.0 output
	TA0CTL = TASSEL__SMCLK + MC_3 + ID__8;       	// SMCLK, up-down mode, timer A interrupt enable, divide clock by 2
	//TA0CCR0 = 62500;								// timer A0 CCR0 interrupt period = 625000*2/(8*1000000) = 1s
	TA0CCR0 = 15625;
	TA0CTL |= TACLR;                                // clear TAR
}

