/**
 * MSP430 and NRF24L01 based WSN
 * @file spib.c
 * @author Robert Fromm
 * @date 06.02.2022
 *
 * Basic command for usage of the SPI module. SPIB is using the UCB0 module of the MSP430G2553.
 * See spib.h file for inline functions spib_send() and spib_transmit().
 * Using inline functions to get more performance.
 */

#include <msp430.h>
#include <stdint.h>

#include "config.h"
#include "pins.h"
#include "spib.h"


/**
 * Initialization of SPIB module.
 */
void spib_init() {
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC; // SPI phase, MSB first, master mode, synchronous
	UCB0BR0 = SPIB_PRESCALER;
	UCB0BR1 = 0;
	UCB0CTL1 = UCSSEL_2;
}
