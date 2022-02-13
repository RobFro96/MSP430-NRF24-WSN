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

static uint8_t data_transmission();

void sensor_mainloop() {
    uint8_t allowed_retries = MAX_RETRIES;

    while (1) {
        data_collector();
        for (out_regs.retries = 0; out_regs.retries < allowed_retries; out_regs.retries++) {
            p_led_h();
            uint8_t result = data_transmission();
            p_led_l();
            nrf24_enter_sleep();
            if (result) {
                break;
            }

            isr_delay(RETRY_DELAY);
        }

#if TERM_ENABLE
        term_log_begin();
        term_print("Communication ");
        term_uint(out_regs.retries + 1, 2);
        term_putchar('/');
        term_uint(allowed_retries, 2);
        term_print("  successful? ");
        term_test_result(out_regs.retries < allowed_retries);
        term_end();
#endif

        if (out_regs.retries < allowed_retries) {
            // success
            if (allowed_retries < MAX_RETRIES)
                allowed_retries++;
        } else {
            // no communication
            if (allowed_retries > MIN_RETRIES)
                allowed_retries--;
        }


        for (uint8_t i = 0; i < 5; i++) {
            isr_delay(0x8000);
        }
    }
}

static uint8_t data_transmission() {
    nrf24_enter_tx();
    nrf24_tx_data(sizeof(out_regs_t), (uint8_t*) &out_regs);
    uint8_t result = nrf24_wait_on_finished(655);
    if (!result)
        return 0;

    nrf24_enter_rx();
    result = nrf24_wait_on_finished(RX_INTERVAL);
    if (!result)
        return 0;
    nrf24_rx_download();
    return 1;
}
