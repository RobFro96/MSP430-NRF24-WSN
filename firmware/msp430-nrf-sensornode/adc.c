/**
 * MSP430 and NRF24L01 based WSN
 * @file adc.c
 * @author Robert Fromm
 * @date 11.02.2022
 */

#include <msp430.h>
#include <stdint.h>

#include "pins.h"
#include "adc.h"

uint16_t adc_measure_vbat(adc_reference_t ref) {
    ADC10CTL1 = INCH_11 + ADC10DIV_3;
    ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ref;
    p_delay_us(30);
    ADC10CTL0 |= ENC + ADC10SC;

    while (ADC10CTL1 & BUSY) {
    }

    uint16_t adc_result = ADC10MEM;
    ADC10CTL0 &= ~(ENC);
    ADC10CTL0 &= ~(ADC10ON + REFON);
    return adc_result;
}

uint16_t adc_measure_temperature() {
    ADC10CTL1 = INCH_10 + ADC10DIV_3;         // Temp Sensor ADC10CLK/4
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON;
    p_delay_us(30);
    ADC10CTL0 |= ENC + ADC10SC;

    while (ADC10CTL1 & BUSY) {
    }

    uint16_t adc_result = ADC10MEM;
    ADC10CTL0 &= ~(ENC);
    ADC10CTL0 &= ~(ADC10ON + REFON);
    return adc_result;
}
