#ifndef SPIA_H_
#define SPIA_H_

#include "config.h"

#if SPIA_PRESCALER == 1
#define SPIA_CYCLES_PER_BYTE (8)
#else
#define SPIA_CYCLES_PER_BYTE (9 * SPIA_PRESCALER)
#endif
#define SPIA_SEND_CYCLES (4)

void spia_init();

/**
 * Inline function for a full SPI transaction. Sending and receiving a full byte at the same time.
 * For a faster transaction, use function spia_send if return value doesn't matter.
 *
 * @param data_out byte to be sent.
 * @return received byte
 */
inline char spia_transmit(char data_out) {
#if SPIA_USE_FLAG_POLLING
	while (!(IFG2 & UCA0TXIFG)) {
	}
#endif
	UCA0TXBUF = data_out;
#if SPIA_USE_FLAG_POLLING
	while (UCA0STAT & UCBUSY) {
	}
#else
	__delay_cycles(SPIA_CYCLES_PER_BYTE);
#endif
	char data_in = UCA0RXBUF;
	return data_in;
}

/**
 * Inline function for sending a single byte of SPI. The received byte is discarded.
 *
 * @param data_out byte to be sent.
 */
inline void spia_send(char data_out) {
#if SPIA_USE_FLAG_POLLING
	while (!(IFG2 & UCA0TXIFG)) {
	}
#endif
	UCA0TXBUF = data_out;
#if ! SPIA_USE_FLAG_POLLING
	__delay_cycles(SPIA_CYCLES_PER_BYTE - SPIA_SEND_CYCLES);
#endif
}

/**
 * Wait until the CS signal can be released safely.
 */
inline void spia_wait_cs_disable() {
#if SPIA_USE_FLAG_POLLING
	while (UCA0STAT & UCBUSY) {
	}
#else
	__delay_cycles(SPIA_SEND_CYCLES);
#endif
}

#endif /* SPIA_H_ */
