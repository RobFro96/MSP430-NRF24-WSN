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
#include "nrf24.h"
#include "term.h"
#include "sensor.h"
#include "si7021.h"
#include "data_collector.h"

void data_collector() {
    data_collector_rf_values();
    data_collector_vbat();
    data_collector_temperature_internal();
    data_collector_si7021_data();
}

void data_collector_rf_values() {
    out_regs.addr = RF_ADDR_LSB;
    out_regs.retries = 0;

#if TERM_ENABLE
    term_log("");
    term_log_begin();
    term_print("> out_regs.addr = 0x");
    term_hex(out_regs.addr, 2);
    term_end();
#endif
}

void data_collector_vbat() {
    out_regs.vbat = adc_measure_vbat(ADC_REF1_5V);
    if (out_regs.vbat > 990) { // > 2.9 V
        out_regs.vbat = adc_measure_vbat(ADC_REF2_5V) + SENSOR_BIT_ADC_VBAT_2_5V;
    }

#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs.vbat = 0x");
    term_hex(out_regs.vbat, 4);
    term_print("  Vbat=");
    if (out_regs.vbat & SENSOR_BIT_ADC_VBAT_2_5V) {
        term_decimal(out_regs.vbat * 2 * 2500.0f / 1023.0f, 4, 3);
    } else {
        term_decimal(out_regs.vbat * 2 * 1500.0f / 1023.0f, 4, 3);
    }
    term_putchar('V');
    term_end();
#endif
}

void data_collector_temperature_internal() {
#if INTERNAL_TEMPERATURE_ENABLE
    out_regs.temperature_internal = adc_measure_temperature();
#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs.temperature_internal = 0x");
    term_hex(out_regs.temperature_internal, 4);
    term_print("  Temp=");
    term_signed_decimal(out_regs.temperature_internal * 1500.0f / 1023.0f / 0.00355f - (986.0f / 0.00355f), 5, 3);
    term_print("^C");
    term_end();
#endif
    out_regs.temperature_internal = 0xFFFF;
#endif
}

void data_collector_si7021_data() {
#if SI7021_ENABLE
    uint8_t err = si7021_measurement(&out_regs.si7021_temperature, &out_regs.si7021_humidity);
    if (err) {
        out_regs.si7021_temperature = 0xffff;
        out_regs.si7021_humidity = 0xffff;
    }
#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs.si7021_humidity = 0x");
    term_hex(out_regs.si7021_humidity, 4);
    term_print("  RH=");
    term_decimal(si7021_convert_humidity_2decimals(out_regs.si7021_humidity), 5, 2);
    term_print("%");
    term_end();
    term_log_begin();
    term_print("> out_regs.si7021_temperature = 0x");
    term_hex(out_regs.si7021_temperature, 4);
    term_print("  T=");
    term_signed_decimal(si7021_convert_temperature_2decimals(out_regs.si7021_temperature), 5, 2);
    term_print("^C");
    term_end();
#endif
#else
    out_regs.si7021_temperature = 0xffff;
    out_regs.si7021_humidity = 0xffff;
#endif
}
