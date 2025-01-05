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
#include "spirit.h"
#include "term.h"
#include "sensor.h"

int16_t out_regs[16];
in_regs_t in_regs;

static uint8_t data_transmission();
static void display_in_regs();

void sensor_mainloop() {
    uint8_t allowed_retries = MAX_RETRIES;

    while (1) {
        data_collector();
        uint8_t retries = 0;
        for (retries = 0; retries < allowed_retries; retries++) {
            uint8_t result = data_transmission();
            if (result) {
                break;
            }

            out_regs[OUT_REGS_RETRIES_INDEX]++;
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
            out_regs[OUT_REGS_RETRIES_INDEX] = 0;
            if (allowed_retries < MAX_RETRIES)
                allowed_retries++;

            p_led_h();
            isr_delay_ms(10);
            p_led_l();

            display_in_regs();

        } else {
            // no communication
            if (allowed_retries > MIN_RETRIES)
                allowed_retries--;
        }

        // Sleep
        uint16_t sleep_duration = (in_regs.testing == MY_ADDRESS) ? SLEEP_TESTING : SLEEP_DEFAULT;
        for (uint16_t i = 0; i < sleep_duration; i++) {
            isr_delay_ms(1000);
        }
    }
}

static uint8_t data_transmission() {
    const uint8_t control_mask[] = CONTROL_MASK;

    spirit_wakeup();
    spirit_tx_set_ctrl(4, CONTROL_WORD);
    spirit_tx_set_addr(0); // always transmit to central
    spirit_rx_set_ctrl(4, control_mask, CONTROL_WORD);
    spirit_rx_set_addr(MY_ADDRESS);


    spirit_tx_data(OUT_PAYLOAD_LEN * 2, (uint8_t*) out_regs);
    uint8_t result = spirit_tx_wait_on_finished(10);
    if (!result)
        return 0;

    spirit_rx_start();
    result = spirit_rx_timeout(RX_INTERVAL);
    spirit_rx_stop();
    spirit_sleep();

    if (!result)
        return 0;

    memcpy(&in_regs, spirit_msg.data, sizeof(in_regs));

    return 1;
}

static void display_in_regs() {
#if TERM_ENABLE
    term_log_begin();
    term_print("< in_regs.testing = ");
    term_hex(in_regs.testing, 2);
    term_end();
#endif
}
