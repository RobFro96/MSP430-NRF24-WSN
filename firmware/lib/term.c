/**
 * MSP430 and NRF24L01 based WSN
 * @file term.c
 * @author Robert Fromm
 * @date 06.02.2022
 *
 * Sending text over UART to a PC terminal program. Used for debugging program.
 * Command line input for Linux PC: $ tio -b 9600 -m INLCRNL -t /dev/ttyACM1
 */
#include <msp430.h>
#include <stdint.h>

#include "pins.h"
#include "spia.h"
#include "config.h"
#include "term.h"


/**
 * Inline function for leaving the UART mode of the used UCA module.
 * Calling initialization function of SPI mode
 */
#if TERM_ENABLE
static inline void term_disable() {
	while (!(IFG2 & UCA0TXIFG)) {
	}
	p_delay_ms(1);
	spia_init();
}
#endif

/**
 * Initializing UCA0 module for UART use.
 * Using baudrate of 9600 baud. Other baudrates must be configuared manually3
 */
void term_init() {
#if TERM_ENABLE
	UCA0CTL1 |= UCSWRST;
	UCA0CTL0 = 0;
	UCA0CTL1 |= UCSSEL_2;

	// 9600 Baud
#if P_CLOCK_FREQ_MHZ == 1
	UCA0BR0 = 104;
	UCA0BR1 = 0;
	UCA0MCTL = (1 << 1);
#elif P_CLOCK_FREQ_MHZ == 8
	UCA0BR0 = 65;
	UCA0BR1 = 3;
	UCA0MCTL = (2 << 1);
#else
#error "Define UART baud rate control register values for selected clock frequency."
#endif

	UCA0CTL1 &= ~UCSWRST;
	p_delay_ms(1);
#endif
}

/**
 * Send character over uart.
 *
 * @param c character to be sent
 */
void term_putchar(char c) {
#if TERM_ENABLE
	while (!(IFG2 & UCA0TXIFG)) {
	}
	UCA0TXBUF = c;
#endif
}

/**
 * Send a string over uart. End of string is defined by null-character '\0'.
 *
 * @param str string to be sent
 */
void term_print(char *str) {
#if TERM_ENABLE
	while (*str != 0) {
		while (!(IFG2 & UCA0TXIFG)) {
		}
		UCA0TXBUF = *str;
		*str++;
	}
#endif
}

/**
 * Function to start a logging message.
 * Initializing terminal and sending a green L
 */
void term_log_begin() {
#if TERM_ENABLE
	term_init();
    // term_print("\e[1m\e[32mL\e[0m ");
#endif
}

/**
 * Function to end a logging message.
 * Sending newline character, calling function term_disable()
 */
void term_end() {
#if TERM_ENABLE
	term_putchar('\r');
	term_putchar('\n');
	term_disable();
#endif
}

/**
 * Sending a complete logging message.
 *
 * @param str string to be sent
 */
void term_log(char *str) {
#if TERM_ENABLE
	term_log_begin();
	term_print(str);
	term_end();
#endif
}

/**
 * Sending a unsigned number over uart. Function needs a static number of digits.
 * Number is always printed right-justified.
 *
 * @param number number to be sent.
 * @param digit_count number of digits
 */
void term_uint(uint16_t number, uint8_t digit_count) {
#if TERM_ENABLE
	char str[6];
	for (uint8_t i = 0; i < digit_count; i++) {
		if (number == 0 && i != 0) {
			str[digit_count - i - 1] = ' ';
		} else {
			str[digit_count - i - 1] = '0' + (number % 10);
			number /= 10;
		}
	}
	str[digit_count] = 0;
	term_print(str);
#endif
}

/**
 * Sending a signed number over uart. Function needs a static number of digits.
 * The sign is added as an extra character and is either space or minus.
 *
 * @param number number to be sent
 * @param digit_count number of digits
 */
void term_int(int16_t number, uint8_t digit_count) {
#if TERM_ENABLE
	if (number < 0) {
		term_putchar('-');
		term_uint(-number, digit_count);
	} else {
		term_putchar(' ');
		term_uint(number, digit_count);
	}
#endif
}

/**
 * Sending a number in its binary form. Function needs a static number of digits.
 *
 * @param number number to be sent
 * @param digit_count number of binary digits
 */
void term_binary(uint16_t number, uint8_t digit_count) {
#if TERM_ENABLE
	char str[17];
	for (uint8_t i = 0; i < digit_count; i++) {
		str[digit_count - i - 1] = '0' + (number & 1);
		number >>= 1;
	}

	str[digit_count] = 0;
	term_print(str);
#endif
}

/**
 * Sending a number in its hexadecimal form. Function needs a static number of digits.
 *
 * @param number number to be sent
 * @param digit_count number of hexadecimal digits
 */
void term_hex(uint16_t number, uint8_t digit_count) {
#if TERM_ENABLE
	char str[9];
	for (uint8_t i = 0; i < digit_count; i++) {
		uint8_t digit = number & 0xf;
		if (digit < 10) {
			str[digit_count - i - 1] = '0' + digit;
		} else {
			str[digit_count - i - 1] = 'a' + digit - 10;
		}
		number >>= 4;
	}

	str[digit_count] = 0;
	term_print(str);
#endif
}

/**
 * Sending the result of a boolean test. If result is true a green P is printed, else a red X.
 *
 * @param result result of test as boolean
 */
void term_test_result(uint8_t result) {
#if TERM_ENABLE
	if (result) {
		term_print("\e[1m\e[32mP\e[0m");
	} else {
		term_print("\e[1m\e[31mX\e[0m");
	}
#endif
}

void term_wait_and_clear() {
#ifdef TERM_ENABLE
    p_delay_ms(1000);
    term_init();
    for (uint8_t i = 0; i < 10; i++) {
        term_print("\r\n");
    }
    term_end();
#endif
}
