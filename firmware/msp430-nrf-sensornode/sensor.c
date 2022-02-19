/**
 * MSP430 and NRF24L01 based WSN
 * @file sensor.c
 * @author Robert Fromm
 * @date 11.02.2022
 */

#include <msp430.h>
#include <stdint.h>

#include "isr.h"
#include "pins.h"
#include "data_collector.h"
#include "nrf24.h"
#include "term.h"
#include "sensor.h"

out_regs_t out_regs;
in_regs_t in_regs;

static uint8_t data_transmission();
static void display_in_regs();
static uint8_t is_my_led_disabled();

void sensor_mainloop() {
    uint8_t allowed_retries = MAX_RETRIES;

    while (1) {
        data_collector();
        uint8_t retries = 0;
        for (retries = 0; retries < allowed_retries; retries++) {
            uint8_t result = data_transmission();
            nrf24_enter_sleep();
            if (result) {
                break;
            }

            out_regs.retries++;
            isr_delay_ms(RETRY_DELAY);
        }

#if TERM_ENABLE
        term_log_begin();
        term_print("Communication ");
        term_uint(retries + 1, 2);
        term_putchar('/');
        term_uint(allowed_retries, 2);
        term_print("  successful? ");
        term_test_result(retries < allowed_retries);
        term_end();
#endif

        if (retries < allowed_retries) {
            // success
            out_regs.retries = 0;
            if (allowed_retries < MAX_RETRIES)
                allowed_retries++;

            if (!is_my_led_disabled()) {
                p_led_h();
                isr_delay_ms(10);
                p_led_l();
            }

            display_in_regs();

        } else {
            // no communication
            if (allowed_retries > MIN_RETRIES)
                allowed_retries--;
        }

        // Sleep
        uint16_t sleep_duration = (in_regs.testing == RF_ADDR_LSB) ? SLEEP_TESTING : SLEEP_DEFAULT;
        for (uint16_t i = 0; i < sleep_duration; i++) {
            isr_delay_ms(1000);
        }
    }
}

static uint8_t data_transmission() {
    nrf24_enter_tx();
    nrf24_tx_data(sizeof(out_regs_t), (uint8_t*) &out_regs);
    uint8_t result = nrf24_wait_on_finished(10);
    if (!result)
        return 0;

    nrf24_enter_rx();
    result = nrf24_wait_on_finished(RX_INTERVAL);
    if (!result)
        return 0;

    nrf24_rx_download(sizeof(in_regs), (uint8_t*) &in_regs);
    return 1;
}

static void display_in_regs() {
#if TERM_ENABLE
    term_log_begin();
    term_print("< in_regs.led_en = ");
    for (uint8_t i = 0; i < sizeof(in_regs.led_dis); i++) {
        term_hex(in_regs.led_dis[i], 2);
        term_putchar(' ');
    }
    term_end();

    term_log_begin();
    term_print("< in_regs.testing = ");
    term_hex(in_regs.testing, 2);
    term_end();

    term_log_begin();
    term_print("< in_regs.status_led = ");
    term_hex(in_regs.status_led, 2);
    term_end();
#endif
}

static uint8_t is_my_led_disabled() {
    uint8_t index = RF_ADDR_LSB << 3; // divide by 8
    uint8_t bit = RF_ADDR_LSB & 7; // rest modulo 8
    return in_regs.led_dis[index] & (1 << bit);
}
