/**
 * MSP430 and NRF24L01 based WSN
 * @file config.h
 * @author Robert Fromm
 * @date 06.02.2022
 *
 * Basic command for usage of the SPI module. SPIA is using the UCA0 module of the MSP430G2553.
 * See spia.h file for inline functions spia_send() and spia_transmit().
 * Using inline functions to get more performance.
 */

#include <msp430.h>
#include <stdint.h>

#include "config.h"
#include "pins.h"
#include "spia.h"


/**
 * Initialization of SPIA module.
 */
void spia_init() {
	UCA0CTL1 |= UCSWRST;
	UCA0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC; // SPI phase, MSB first, master mode, synchronous
	UCA0BR0 = SPIA_PRESCALER;
	UCA0BR1 = 0;
	UCA0CTL1 = UCSSEL_2;
}
