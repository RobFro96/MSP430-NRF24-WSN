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


    spirit_tx_data(sizeof(out_regs_t), (uint8_t*) &out_regs);
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
    uint8_t index = MY_ADDRESS << 3; // divide by 8
    uint8_t bit = MY_ADDRESS & 7; // rest modulo 8
    return in_regs.led_dis[index] & (1 << bit);
}
