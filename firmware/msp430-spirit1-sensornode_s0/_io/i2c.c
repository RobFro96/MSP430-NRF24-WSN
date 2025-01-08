/**
 * MSP430 and NRF24L01 based WSN
 * @file i2c.c
 * @author Robert Fromm, HTWK Leipzig
 * @date 05.06.2020
 *
 * Synchronous functions for write and read requests of the I²C bus of the MSP430.
 * The MSP430 is used in the master mode. The Watchdog Timer of the MSP430 is used
 * to stop I²C interactions in case of the slave not responding.
 */

#include <msp430.h>
#include <stdint.h>

#include "config.h"
#include "i2c.h"

static inline uint8_t flag_polling_with_wdt(volatile uint8_t *reg, uint16_t flag);
static inline uint8_t flag_polling_with_wdt_cleared(volatile uint8_t *reg, uint16_t flag);
static inline void start_wdt();
static inline void stop_wdt();

/**
 * Initialization of the USCIB0 module as I²C master.
 * Make sure to connect the corresponding pins by setting the PxSEL register.
 */
void i2c_init() {
	UCB0CTL1 |= UCSWRST;				// Soft-Reset of I2C module
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;	// master, I2C, synchronous
	UCB0BR0 = I2C_PRESCALER;
	UCB0BR1 = 0;
	UCB0CTL1 = UCSSEL_2; // Reset-Loslassen, SMCLK
}

/**
 * I²C write request.
 *
 * @param address 7-bit slave adresse
 * @param data data pointer
 * @param len number of bytes to transfer
 * @return returns 1 in case of an error
 */
uint8_t i2c_write(uint8_t address, const uint8_t* data, uint8_t len) {
	// Warten, bis Bus frei
	if (flag_polling_with_wdt_cleared(&UCB0CTL1, UCTXSTP)) {
		return 1;
	}

	UCB0I2CSA = address; // Slave Adresse setzen
	UCB0CTL1 |= UCTR + UCTXSTT;		 // W-Mode, Start der Übertragung

	// Senden der Datenbytes
	for (uint8_t i = 0; i < len; i++) {
		if (flag_polling_with_wdt(&IFG2, UCB0TXIFG))
			return 1;

		UCB0TXBUF = data[i];
	}

	// Senden der Stoppsequenz
	if (flag_polling_with_wdt(&IFG2, UCB0TXIFG))
		return 1;
	UCB0CTL1 |= UCTXSTP;

	stop_wdt();
	return 0;
}

/**
 * I²C read request
 *
 * @param address 7-bit slave address
 * @param data data pointer
 * @param len number of bytes
 * @return returns 1 in case of an error
 */
uint8_t i2c_read(uint8_t address, uint8_t* data, uint8_t len) {
	// Warten, bis Bus frei
	if (flag_polling_with_wdt_cleared(&UCB0CTL1, UCTXSTP)) {
		return 1;
	}

	UCB0I2CSA = address; // Slave Adresse setzen
	UCB0CTL1 &= ~UCTR;	// R-Mode
	UCB0CTL1 |= UCTXSTT; // Start der Übertragung

	// Warte, bis Startsequenz gesendet
	if (flag_polling_with_wdt_cleared(&UCB0CTL1, UCTXSTT))
		return 1;

	// Empfangen der Datenbytes
	for (uint8_t i = 0; i < len; i++) {
		if (i == len - 1) {
			// Setzen der Stop-Bits, während des Empfangen des letzten Bytes
			UCB0CTL1 |= UCTXSTP;
		}

		if (flag_polling_with_wdt(&IFG2, UCB0RXIFG))
			return 1;

		data[i] = UCB0RXBUF;
	}

	stop_wdt();
	return 0;
}

/**
 * Function for flag polling, waiting for the bit in register to go high, or a timeout is generated
 * by the watchdog timer. The timeout duration is defined in the function start_wdt()
 *
 * @param reg pointer in register
 * @param flag bits to turn high
 * @return returns 0, when the timeout was not reached.
 */
static inline uint8_t flag_polling_with_wdt(volatile uint8_t *reg, uint16_t flag) {
	start_wdt();
	while (!(*reg & flag) && !(IFG1 & WDTIFG)) {
	}
	return IFG1 & WDTIFG;
}

static inline uint8_t flag_polling_with_wdt_cleared(volatile uint8_t *reg, uint16_t flag) {
	start_wdt();
	while ((*reg & flag) && !(IFG1 & WDTIFG)) {
	}
	return IFG1 & WDTIFG;
}

/**
 * Initialization of the Watchdog Timer in Interval mode, no interrupt enable.
 * WDTIFG is set after the timeout.
 * Timeout is set after: ACLK /64 = 2 ms.
 */
static inline void start_wdt() {
	WDTCTL = WDTPW + WDTTMSEL + WDTCNTCL + WDTSSEL + WDTIS0 + WDTIS1;
	IFG1 &= ~WDTIFG;
}

/**
 * Hold the watchdog timer.
 */
static inline void stop_wdt() {
	WDTCTL = WDTPW + WDTHOLD;
}

