/**
 * MSP430 and NRF24L01 based WSN
 * @file main.c
 * @author Robert Fromm
 * @date 06.02.2022
 */
#include <msp430.h>
#include <stdint.h>

#include "config.h"
#include "pins.h"
#include "spia.h"
#include "term.h"
#include "nrf24.h"
#include "test_nrf24.h"
#include "isr.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    p_setup();
    spia_init();
    __enable_interrupt();
    term_wait_and_clear();

    nrf24_init();

    test_nrf24_registers();

#if PROG_ID == 'T'
    while (1) {
        test_nrf24_tx();

        for (uint8_t i = 0; i < 1; i++) {
            isr_delay(0x2000);
        }
    }
#else
    test_nrf24_rx_loop();
#endif
}
