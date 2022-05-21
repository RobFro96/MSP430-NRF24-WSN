#ifndef SPIB_H_
#define SPIB_H_

#include "config.h"

#if SPIB_PRESCALER == 1
#define SPIB_CYCLES_PER_BYTE (8)
#else
#define SPIB_CYCLES_PER_BYTE (9 * SPIB_PRESCALER)
#endif
#define SPIB_SEND_CYCLES (4)

void spib_init();

/**
 * Inline function for a full SPI transaction. Sending and receiving a full byte at the same time.
 * For a faster transaction, use function spib_send if return value doesn't matter.
 *
 * @param data_out byte to be sent.
 * @return received byte
 */
inline char spib_transmit(char data_out) {
#if SPIB_USE_FLAG_POLLING
	while (!(IFG2 & UCB0TXIFG)) {
	}
#endif
	UCB0TXBUF = data_out;
#if SPIB_USE_FLAG_POLLING
	while (UCB0STAT & UCBUSY) {
	}
#else
	__delay_cycles(SPIB_CYCLES_PER_BYTE);
#endif
	char data_in = UCB0RXBUF;
	return data_in;
}

/**
 * Inline function for sending a single byte of SPI. The received byte is discarded.
 *
 * @param data_out byte to be sent.
 */
inline void spib_send(char data_out) {
#if SPIB_USE_FLAG_POLLING
	while (!(IFG2 & UCB0TXIFG)) {
	}
#endif
	UCB0TXBUF = data_out;
#if ! SPIB_USE_FLAG_POLLING
	__delay_cycles(SPIB_CYCLES_PER_BYTE - SPIB_SEND_CYCLES);
#endif
}

/**
 * Wait until the CS signal can be released safely.
 */
inline void spib_wait_cs_disable() {
#if SPIB_USE_FLAG_POLLING
	while (UCB0STAT & UCBUSY) {
	}
#else
	__delay_cycles(SPIB_SEND_CYCLES);
#endif
}

#endif /* SPIB_H_ */
