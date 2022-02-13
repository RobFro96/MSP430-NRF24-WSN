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
#include "spib.h"
#include "term.h"
#include "nrf24.h"
#include "test_nrf24.h"
#include "isr.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    p_setup();
    spib_init();
    __enable_interrupt();
    term_init();
    term_wait_and_clear();

    nrf24_init();
    nrf24_set_addr(NRF24_RX_ADDR_P0, 0);
    nrf24_set_addr(NRF24_TX_ADDR, 1); // for testing

    // TESTS
#if TERM_ENABLE
    test_nrf24_registers();
#endif

    test_nrf24_rx_loop();
}
