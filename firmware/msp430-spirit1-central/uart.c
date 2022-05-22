/**
 * MSP430 and NRF24L01 based WSN
 * @file main.c
 * @author Robert Fromm
 * @date 20.02.2022
 */
#include <msp430.h>
#include <stdint.h>

#include "pins.h"
#include "isr.h"
#include "uart.h"

uint8_t uart_rx_buffer[67]; //!> data input buffer
volatile static uint8_t rx_payload_len; //!> length of payload of current message
volatile static uint8_t rx_buffer_index; //!> index of next character in buffer

/**
 * Initialization
 */
void uart_init() {
	UCA0CTL1 |= UCSWRST;
	UCA0CTL0 = 0;
	UCA0CTL1 |= UCSSEL_2;

#if P_CLOCK_FREQ_MHZ == 8
	// 115200 Baud
    UCA0BR0 = 69;
	UCA0BR1 = 0;
    UCA0MCTL = (4 << 1);
#else
#error "Define UART baud rate control register values for selected clock frequency."
#endif

	UCA0CTL1 &= ~UCSWRST;
	p_delay_ms(1);

	UC0IE |= UCA0RXIE;

	rx_buffer_index = 0;
	rx_payload_len = 0;
}

/**
 * Transmit character synchronously.
 * @param c character
 */
void uart_send(uint8_t c) {
	while (!(IFG2 & UCA0TXIFG)) {
	}
	UCA0TXBUF = c;
}

/**
 * UART RX interrupt
 */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
	if (IFG2 & UCA0RXIFG) {
		IFG2 &= ~UCA0RXIFG;
		char c = UCA0RXBUF;

		// store current byte
		if (((c == UART_START_BYTE) && (rx_buffer_index == 0)) || (rx_buffer_index > 0)) {
			uart_rx_buffer[rx_buffer_index] = c;
			rx_buffer_index++;
		}

		// read playload length
		if (rx_buffer_index == UART_LEN + 1) {
			rx_payload_len = c;
		}

		// end of message
		if (rx_buffer_index == rx_payload_len + UART_MESSAGE_EXTRA_LEN) {
			rx_buffer_index = 0;
            isr_flags |= ISR_UART_IRQ;
			__low_power_mode_off_on_exit();
		}
	}
}
