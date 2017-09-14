#include <msp430.h>
#include <stdio.h>
#include <intrinsics.h>
#include "uart.h"

void uartConfig(){

	P9SEL |= 0x0C;							// Assign P9.2 to UCA2TXD and P9.3 to UCA2RXD
	UCA2CTL1 |= UCSWRST;					// Put state machine in reset
	UCA2CTL1 |= UCSSEL__ACLK;				// CLK = ACLK (16 MHz)
	UCA2BR0 = 0x82;							// baud rate selection 9600
	UCA2BR1 = 0x06;
	UCA2MCTL = UCBRS_6|UCBRF_0;				// Modulation UCBRSx=6, UCBRFx=0
	UCA2CTL1 &= ~UCSWRST;					// Initialize USCI state machine
}

void uartTX(uint8_t *tx_data){

	uint8_t i = 0;				  	      // used to determine when array is finished
	while(tx_data[i])                     // Increment through array, look for null pointer (0) at end of string
	{
		while ((UCA2STAT & UCBUSY));      // Wait if line TX/RX module is busy with data
		UCA2TXBUF = tx_data[i];           // Send out element i of tx_data array on UART bus
		i++;                              // Increment variable for array address
	}
}


void uartTXFloat(volatile float c){

    uint8_t string[30];

    if(c < 0){
    	uartTX("-");
    	c = -c;
    }

    sprintf(string, "%f", c);
	uartTX(string);
}

