/**
 * MSP430 and NRF24L01 based WSN
 * @file term.c
 * @author Robert Fromm
 * @date 06.02.2022
 */

#include <msp430.h>
#include <stdint.h>

#include "pins.h"
#include "nrf24.h"
#include "term.h"
#include "isr.h"
#include "test_nrf24.h"

void test_nrf24_registers() {
    term_log("\e[1mtest_nrf24_registers()\e[0m");
    for (uint8_t i = 0; i < nrf24_default_profile_count; i++) {
        uint8_t val = nrf24_read_register(nrf24_default_profile[i].reg);

        term_log_begin();
        term_putchar('R');
        term_uint(nrf24_default_profile[i].reg + 100, 2);
        term_putchar('=');
        term_binary(val, 8);
        term_putchar(' ');
        term_test_result(val == nrf24_default_profile[i].val);
        term_end();
    }
}

void test_nrf24_tx() {
    term_log("\e[1mtest_nrf24_tx()\e[0m");

    p_led_h();
    nrf24_enter_tx();
    nrf24_tx_data(5, "Hello");
    uint8_t result_tx = nrf24_wait_on_finished(655);

    nrf24_enter_rx();
    uint8_t result_rx = nrf24_wait_on_finished(655);
    nrf24_rx_download();

    nrf24_enter_sleep();

    term_log_begin();
    term_print("TX: ");
    term_test_result(result_tx);
    term_print("  RX: ");
    term_test_result(result_rx);
    if (result_rx) {
        term_print("  ");
        for (uint8_t i = 0; i < 16; i++) {
            term_hex(nrf24_rx_data[i], 2);
            term_putchar(' ');
        }
        term_print("...");
    }
    term_end();
    p_led_l();
}

void test_nrf24_rx_loop() {
    term_log("\e[1mtest_nrf24_rx_loop()\e[0m");

    nrf24_enter_rx();
    while (1) {
        if (isr_flag_isset_with_clear(ISR_NRF24_IRQ) || !p_nrf_irq_val()) {
            uint8_t status = nrf24_read_status_and_clear();
            if (status & NRF24_RX_DR) {
                nrf24_rx_download();

                p_delay_ms(1);

                // Send ACK
                nrf24_enter_tx();
                nrf24_tx_data(4, "ACKN");
                nrf24_wait_on_finished(327);

                nrf24_enter_rx();

                p_led_h();
                term_log_begin();
                for (uint8_t i = 0; i < 32; i++) {
                    term_hex(nrf24_rx_data[i], 2);
                    term_putchar(' ');
                }
                term_end();
                p_led_l();
            }
            nrf24_flush();
        } else {
            __low_power_mode_4();
        }
    }
}
