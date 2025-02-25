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
    data_collector_si7021_data();
    data_collector_digital_in();
    data_collector_resistor_div();
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

void data_collector_si7021_data() {
#if SI7021_INDEX
    uint16_t temp;
    uint16_t hum;
    uint8_t err = si7021_measurement(&temp, &hum);

    if (err)
        return;

    out_regs[SI7021_INDEX] = si7021_convert_temperature_2decimals(temp);
    out_regs[SI7021_INDEX + 1] = si7021_convert_humidity_2decimals(hum);

#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs.temperature (Si7021) = ");
    term_int(out_regs[SI7021_INDEX], 5);
    term_end();
    term_log_begin();
    term_print("> out_regs.humidity (Si7021) = ");
    term_int(out_regs[SI7021_INDEX + 1], 5);
    term_end();
#endif
#endif
}

void data_collector_digital_in() {
#if DIGITAL_IN_INDEX
    p_1_3_pullup();
    p_delay_us(10);
    if (p_1_3_val()) {
        // open, H
        out_regs[DIGITAL_IN_INDEX] = 0;
    } else {
        // close, L
        out_regs[DIGITAL_IN_INDEX] = 1;
    }
    p_1_3_pulldown();
#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs.digital_in = ");
    term_int(out_regs[DIGITAL_IN_INDEX], 2);
    term_end();
#endif
#endif
}

void data_collector_resistor_div() {
#if RESISTOR_DIV_INDEX
    p_1_5_h();
    p_1_3_pullno();
    ADC10AE0 |= BIT3;
    p_delay_us(100);

    ADC10CTL1 = INCH_3 + ADC10DIV_3;
    ADC10CTL0 = ADC10SHT_3 + ADC10ON;
    p_delay_us(30);
    ADC10CTL0 |= ENC + ADC10SC;

    while (ADC10CTL1 & BUSY) {
    }

    uint16_t adc_result = ADC10MEM;
    ADC10CTL0 &= ~(ENC);
    ADC10CTL0 &= ~(ADC10ON);

    p_1_5_l();

    if (adc_result < 993) {
        out_regs[RESISTOR_DIV_INDEX] = (1000L * adc_result) / (1024 - adc_result);
    } else {
        out_regs[RESISTOR_DIV_INDEX] = (1000L * 993) / (1024 - 993);
    }

#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs.resistor_div = ");
    term_int(out_regs[RESISTOR_DIV_INDEX], 4);
    term_end();
#endif
#endif
}

