/**
 * @file data_collector.c
 * @author Robert Fromm
 * @date 12.02.2022
 */

#include <msp430.h>
#include <stdint.h>

#include "pins.h"
#include "adc.h"
#include "config.h"
#include "isr.h"
#include "spirit.h"
#include "term.h"
#include "sensor.h"
#include "si7021.h"
#include "data_collector.h"

void data_collector() {
    for (uint8_t i = 0; i < OUT_PAYLOAD_LEN; i++) {
        out_regs[i] = 0;
    }

    data_collector_rf_values();
    data_collector_vbat();
    data_collector_temperature_internal();
    data_collector_s0();
}

void data_collector_rf_values() {
    out_regs[OUT_REGS_ADDR_INDEX] = MY_ADDRESS;

#if TERM_ENABLE
    term_log("");
    term_log_begin();
    term_print("> out_regs[0] = ");
    term_int(out_regs[OUT_REGS_ADDR_INDEX], 3);
    term_end();
#endif
}

void data_collector_vbat() {
    int16_t vbat = adc_measure_vbat(ADC_REF1_5V);
    if (vbat > 990) { // > 2.9 V
        vbat = adc_measure_vbat(ADC_REF2_5V);
        out_regs[OUT_REGS_VBAT_INDEX] = vbat * 2 * 2500.0f / 1023.0f;
    } else {
        out_regs[OUT_REGS_VBAT_INDEX] = vbat * 2 * 1500.0f / 1023.0f;
    }

#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs[2] = ");
    term_int(out_regs[OUT_REGS_VBAT_INDEX], 5);
    term_end();
#endif
}

void data_collector_temperature_internal() {
#if INTERNAL_TEMPERATURE_INDEX
    int16_t temp = adc_measure_temperature();
    out_regs[INTERNAL_TEMPERATURE_INDEX] = temp * 150.0f / 1023.0f / 0.00355f - (98.6f / 0.00355f);
#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs.temperature (internal) = ");
    term_int(out_regs[INTERNAL_TEMPERATURE_INDEX], 5);
    term_end();
#endif
#endif
}

volatile uint32_t timer_overflows;
volatile uint32_t last_interrupt;
volatile uint32_t s0_delta;
volatile uint32_t s0_counter;

void data_collector_init_timer() {
    TA1CTL = TASSEL_1 + MC_2 + TACLR + TAIE;
    TA1CCTL0 = CM_1 + CCIS_2 + CAP;
    timer_overflows = 0;
    last_interrupt = 0;
    s0_delta = 0xFFFFFFFF;
    s0_counter = 0;
}

void data_collector_s0() {
    __disable_interrupt();
    TA1CCTL0 ^= CCIS0;
    TA1CCTL0 ^= CCIS0;
    uint32_t now = timer_overflows + TA1CCR0;
    uint32_t new_delta = now - last_interrupt;

    if (new_delta > 32768UL * 60UL * 10UL) {
        // too long pulses
        s0_delta = 0xFFFFFFFF;
    }

    if (new_delta < s0_delta) {
        new_delta = s0_delta;
    }

    out_regs[S0_DELTA_INDEX] = new_delta >> 16;
    out_regs[S0_DELTA_INDEX + 1] = new_delta;
    out_regs[S0_COUNTER_INDEX] = s0_counter >> 16;
    out_regs[S0_COUNTER_INDEX + 1] = s0_counter;

    __enable_interrupt();


#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs.s0_counter = 0x");
    term_hex(out_regs[S0_COUNTER_INDEX], 4);
    term_hex(out_regs[S0_COUNTER_INDEX + 1], 4);
    term_end();

    term_log_begin();
    term_print("> out_regs.s0_delta = 0x");
    term_hex(out_regs[S0_DELTA_INDEX], 4);
    term_hex(out_regs[S0_DELTA_INDEX + 1], 4);
    term_end();
#endif
}


#pragma vector=TIMER1_A1_VECTOR
__interrupt void TimerA1_ISR1() {
    switch (TA1IV) {
    case 2:
        // CCR1 Interrupt
        break;
    case 4:
        // CCR2 Interrupt
        break;
    case 10:
        // Timer Overflow
        timer_overflows += 0x10000UL;
        break;
    }
}
