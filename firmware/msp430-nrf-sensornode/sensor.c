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
#include "sensor.h"

out_regs_t out_regs;

void sensor_mainloop() {
    while (1) {
        p_led_h();
        data_collector();
        p_led_l();

        test_nrf24_tx();

        for (uint8_t i = 0; i < 5; i++) {
            isr_delay(0x8000);
        }
    }
}

