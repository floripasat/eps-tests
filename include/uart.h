#ifndef EPS_SERIAL_TEST_H_
#define EPS_SERIAL_TEST_H_

#include <stdint.h>


/********** UART functions **********/


void uartConfig(void);

void uartTX(uint8_t *tx_data);

void uartTXFloat(float c);

#endif /* EPS_SERIAL_TEST_H_ */
