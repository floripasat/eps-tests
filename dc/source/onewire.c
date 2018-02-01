#include "onewire.h"

/************************************************************************/
/*  - name: outp
 *  - Description: This function sends bit to 1-wire slave.
 *  - input data: int bit
 *  - output data: --
 ***********************************************************************/

void outp(int bit){

	DIR_P_Wire |= BitWire;			//sets P5.5 as output

	if(bit == 0){
		P_WireOUT &= ~BitWire;           //drives P1.0 low
	}
	else{
		//P_WireOUT |= BitWire;           //releases the bus
	DIR_P_Wire &= ~BitWire;
	}
}
/************************************************************************/
/*  - name: inp
 *  - Description: This function reads a bit from the 1-wire slave.
 *  - input data: --
 *  - output data: int result
 ***********************************************************************/

int inp(void){
volatile unsigned int result=0;

	 DIR_P_Wire &= ~BitWire;          //sets pin as input
	 result= P_WireIN & BitWire;         //prepares the bit on pin to be returned

	 return result;		        //returns the bit on pin

	}

/************************************************************************/
/*  - name: OneWireReset
 *  - Description: This function genererates 1-wire reset. Returns 0
 *    if no presence detect was found, return 1 otherwise.
 *  - input data: --
 *  - output data: int result
 ***********************************************************************/

int OneWireReset(void){

	int result=0;

	  outp(1);				        		//drives pin to high
	__delay_cycles(0);			    		// delay of 0
	  outp(0);				        		//drives pin low
	__delay_cycles(clock*0.000480);			//delay of 480us, 8,12MHz* 480us=3897

	  outp(1);						//releases the bus
	  result = inp();			        //prepares the result of present detection to be returned
	  __delay_cycles(clock*0.000070);			//delay of 70us, 8,12MHz*70us=568

	  outp(1);
	__delay_cycles(clock*0.000410);			//delay of 410us, 8,12MHz*410us=3329

	return result;			        //returns the presence detection result

}

/************************************************************************/
/*  - name: OneWireWrite
 *  - Description: this function sends a 1-wire write bit to 1-wire slave.
 *  - input data: int bit
 *  - output data:--
 ***********************************************************************/

void OneWireWrite(int bit){

	if(bit == 1){
		outp(0);				//drives pin low
		__delay_cycles(clock*0.000006);		    //delay of 6us, 8,12 MHz*6us= 44

		outp(1);				//releases the bus
		__delay_cycles(clock*0.000064);		//delay of 64us, 8,12MHz*64us=517

	}else{
		// Write '0' bit
		outp(0);				//drives pin low
		__delay_cycles(clock*0.000060);		//delay of 60us, 8,12MHz*60us=487
		outp(1);				//releases the bus
		__delay_cycles(clock*0.000080);			// delay of 10us, 8MHz*10us=80

	}

}

/************************************************************************/
/*  - name: OneWireReadBit
 *  - Description: This function reads a 1-wire data bit  to 1-wire slave and returns int.
 *  - input data: --
 *  - output data: int result
 ***********************************************************************/

int OneWireReadBit(void){

	int result;
	outp(0);						//drives pin low
	__delay_cycles(clock*0.000006);		        //delay of 6us, 8,12 MHz*6us= 44

	outp(1);						//releases the bus
	__delay_cycles(clock*0.000009);				//delay of 9us, 8,12MHz*9us=73

	result = inp();					//sample the bit from slave
	__delay_cycles(clock*0.000055);			//delay of 55us, 8,12MHz*55us=446

	return result;

}

/************************************************************************/
/*  - name: OWWriteByte
 *  - Description: This function writes a 1-wire data byte.
 *  - input data: int data
 *  - output data: --
 ***********************************************************************/

void OWWriteByte(int data){

	int loop;

	for (loop = 0; loop < 8; loop++)	// Loop to write each bit in the byte, LS-bit first
	{
		OneWireWrite(data & 0x01);

		data >>= 1;						// shift the data byte for the next bit
	}
}

/************************************************************************/
/*  - name: OWReadByte
 *  - Description: This function read a 1-wire data byte and return it
 *  - input data:
 *  - output data: int data
 ***********************************************************************/

uint8_t OWReadByte(void){

	uint8_t loop, result = 0;
	for (loop = 0; loop < 8; loop++)
	{

		result >>= 1;						// shift the result to get it ready for the next bit

		if (OneWireReadBit())			    // if result is one, then set MS bit
			result |= 0x80;
	}
		return result;
}

void DS2784Config(void){

	OneWireReset();							// PROTECTION REGISTER
	OWWriteByte(skipNetAddress);			// slave net address (only one slave on bus, CC is used)
	OWWriteByte(writeRegisterCommand);		// write operation
	OWWriteByte(protection_register);		// register address
	OWWriteByte(0x03);						// value to be written

	OneWireReset();              					// PROTECTOR THRESHOLD REGISTER
	OWWriteByte(skipNetAddress);					// slave net address (only one slave on bus, CC is used)
	OWWriteByte(writeRegisterCommand);				// write operation
	OWWriteByte(protector_threshold_register);		// register address
	OWWriteByte(0x62);								// value to be written

	OneWireReset();							// STATUS REGISTER
	OWWriteByte(skipNetAddress);			// slave net address (only one slave on bus, CC is used)
	OWWriteByte(writeRegisterCommand);		// write operation
	OWWriteByte(status_register);			// register address
	OWWriteByte(0x00);						// value to be written

	OneWireReset();							// CONTROL REGISTER
	OWWriteByte(skipNetAddress);			// slave net address (only one slave on bus, CC is used)
	OWWriteByte(writeRegisterCommand);		// write operation
	OWWriteByte(control_register);			// register address
	OWWriteByte(0x00);						// value to be written

	OneWireReset();										// Overcurrent Threshold REGISTER and current gain calibration MSB
	OWWriteByte(skipNetAddress);						// slave net address (only one slave on bus, CC is used)
	OWWriteByte(writeRegisterCommand);					// write operation
	OWWriteByte(current_gain_MSB_register);				// register address
	OWWriteByte(0x04);									// value to be written

	OneWireReset();								// Overcurrent Threshold REGISTER and current gain calibration LSB
	OWWriteByte(skipNetAddress);				// slave net address (only one slave on bus, CC is used)
	OWWriteByte(writeRegisterCommand);			// write operation
	OWWriteByte(current_gain_LSB_register);		// register address
	OWWriteByte(0x00);							// value to be written

	OneWireReset();										// ACCUMULATED CURRENT - MSB REGISTER
	OWWriteByte(skipNetAddress);						// slave net address (only one slave on bus, CC is used)
	OWWriteByte(writeRegisterCommand);					// write operation
	OWWriteByte(accumulated_current_MSB_register);		// register address
	OWWriteByte(0x25);									// value to be written

	OneWireReset();										// ACCUMULATED CURRENT - LSB REGISTER
	OWWriteByte(skipNetAddress);						// slave net address (only one slave on bus, CC is used)
	OWWriteByte(writeRegisterCommand);					// write operation
	OWWriteByte(accumulated_current_LSB_register);		// register address
	OWWriteByte(0x80);									// value to be written
}

uint8_t DS2784ReadRegister(uint8_t register_address){

	OneWireReset();							// reset one wire to start communication
    OWWriteByte(skipNetAddress);			// slave net address (only one slave on bus, CC is used)
    OWWriteByte(readRegisterCommand);		// read operation
    OWWriteByte(register_address);			// send register address

    return OWReadByte();					// return value read from register

}



