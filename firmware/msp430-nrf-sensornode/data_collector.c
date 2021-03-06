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
#include "bmp180.h"
#include "data_collector.h"

void data_collector() {
    data_collector_rf_values();
    data_collector_vbat();
    data_collector_temperature_internal();
    data_collector_si7021_data();
    data_collector_bmp180_data();
    data_collector_reed();
}

void data_collector_rf_values() {
    out_regs.addr = RF_ADDR_LSB;

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
#else
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

void data_collector_bmp180_data() {
#if BMP180_ENABLE
    bmp_180_cal_param_t cal_param;
    int32_t ut, up;
    uint8_t err = bmp180_get_cal_param(&cal_param);
    err |= bmp180_get_ut(&ut);
    err |= bmp180_get_up(&up);
    if (err) {
        out_regs.bmp180_temperature = 0xffff;
        out_regs.bmp180_pressure = 0xffffffff;
        return;
    }
    out_regs.bmp180_temperature = bmp180_get_temperature(&cal_param, ut);
    out_regs.bmp180_pressure = bmp180_get_pressure(&cal_param, ut, up);
#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs.bmp180_temperature = 0x");
    term_hex(out_regs.bmp180_temperature, 4);
    term_print("  T=");
    term_signed_decimal(out_regs.bmp180_temperature, 4, 1);
    term_print("^C");
    term_end();
    term_log_begin();
    term_print("> out_regs.bmp180_pressure = 0x");
    term_hex(out_regs.bmp180_pressure, 8);
    term_print("  p=");
    term_decimal(out_regs.bmp180_pressure, 6, 2);
    term_print("hPa");
    term_end();
#endif
#else
    out_regs.bmp180_temperature = 0xffff;
    out_regs.bmp180_pressure = 0xffffffff;
#endif
}

void data_collector_reed() {
#if REED_EN
    p_reed_pullup();
    p_delay_us(10);
    if (p_reed_val()) {
        // open, H
        out_regs.reed = REED_H_VALUE;
    } else {
        // close, L
        out_regs.reed = REED_L_VALUE;
    }
    p_reed_pulldown();
#if TERM_ENABLE
    term_log_begin();
    term_print("> out_regs.reed = 0x");
    term_hex(out_regs.reed, 2);
    term_end();
#endif
#else
    out_regs.reed = 0xff;
#endif
}
