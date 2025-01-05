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

uint8_t uart_rx_buffer[64]; //!> data input buffer
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

void uart_print(char *str) {
    while (*str != 0) {
        while (!(IFG2 & UCA0TXIFG)) {
        }
        UCA0TXBUF = *str;
        *str++;
    }
}

void uart_send_hex16(int16_t number) {
    char str[5];
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t digit = number & 0xf;
        if (digit < 10) {
            str[4 - i - 1] = '0' + digit;
        } else {
            str[4 - i - 1] = 'A' + digit - 10;
        }
        number >>= 4;
    }

    str[4] = 0;
    uart_print(str);
}

static int16_t hexchar_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }

    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }

    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }

    return 0;
}

void uart_convert_buffer_to_int16(int16_t *out) {
    for (uint8_t i = 0; i < rx_payload_len / 4; i++) {
        int16_t val = hexchar_to_int(uart_rx_buffer[4 * i]);
        val = val << 4;
        val += hexchar_to_int(uart_rx_buffer[4 * i + 1]);
        val = val << 4;
        val += hexchar_to_int(uart_rx_buffer[4 * i + 2]);
        val = val << 4;
        val += hexchar_to_int(uart_rx_buffer[4 * i + 3]);

        out[i] = val;
    }
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
        if (c == '\n') {
            rx_payload_len = rx_buffer_index;
            rx_buffer_index = 0;
            isr_flags |= ISR_UART_IRQ;
            __low_power_mode_off_on_exit();
        } else {
            uart_rx_buffer[rx_buffer_index] = c;
            rx_buffer_index++;
        }
    }
}
