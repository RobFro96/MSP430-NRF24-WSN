/**
 * MSP430 and NRF24L01 based WSN
 * @file si7021.c
 * @author Robert Fromm
 * @date 11.02.2022
 *
 * Si7021 I2C temperature and humidity sensor
 * Firmware is compatible with SI7021 replacement sensor
 */

#include <msp430.h>
#include <stdint.h>

#include "i2c.h"
#include "config.h"
#include "isr.h"
#include "si7021.h"

/**
 * Start temperature measurement in NOHOLD mode
 * @return 0, if successful
 */
uint8_t si7021_start_temperature() {
    uint8_t data = SI7021_TRIGGER_TEMP_NOHOLD;
    return i2c_write(SI7021_I2C_ADDR, &data, 1);
}

/**
 * Start humidity measurement in NOHOLD mode
 * @return 0, if successful
 */
uint8_t si7021_start_humidity() {
    uint8_t data = SI7021_TRIGGER_HUMI_NOHOLD;
    return i2c_write(SI7021_I2C_ADDR, &data, 1);
}

/**
 * Delay for temperature measurement, use constant SI7021_TIMINGS_LONG to toggle between Si7021 and
 * HTU21D timings
 */
void si7021_temperature_delay() {
#if SI7021_TIMINGS_LONG
    isr_delay_ms(70); // 50 ms + 20 ms
#else
    isr_delay_ms(31); // 11 ms + 20 ms
#endif
}

/**
 * Delay for humidity measurement, use constant SI7021_TIMINGS_LONG to toggle between Si7021 and
 * HTU21D timings
 */
void si7021_humidity_delay() {
#if SI7021_TIMINGS_LONG
    isr_delay_ms(36); // 16 ms + 20 ms
#else
    isr_delay_ms(32); // 12 ms + 20 ms
#endif
}

/**
 * Read the result after a measurement.
 * @param value pointer to result variable
 * @return 0, if successful
 */
uint8_t si7021_read_result(uint16_t* value) {
    uint16_t raw_data;
    uint8_t result = i2c_read(SI7021_I2C_ADDR, (uint8_t *) &raw_data, 2);
    *value = __swap_bytes(raw_data); // sensor gives MSB first
    return result;
}

/**
 * Perform a complete measurement of temperature and humidity
 * @return 0, if all successful
 */
uint8_t si7021_measurement(uint16_t* temperature, uint16_t* humidity) {
    uint8_t status = 0;

    status |= si7021_start_humidity();
    si7021_humidity_delay();
    status |= si7021_read_result(humidity);

    status |= si7021_start_temperature();
    si7021_temperature_delay();
    status |= si7021_read_result(temperature);

    return status;
}

/**
 * Convert raw data to temperature in degree C with two decimals
 * @param raw_data data read from sensor
 * @return 1/100th degree celsius
 */
int16_t si7021_convert_temperature_2decimals(uint16_t raw_data) {
    return 175.72f * raw_data / 655.36f - 4685;
}

/**
 * Convert raw data to relative humdity in percent with two decimals
 * @param raw_data data read from sensor
 * @return 1/100th percent
 */
uint16_t si7021_convert_humidity_2decimals(uint16_t raw_data) {
    return 125.0f * raw_data / 655.36f - 600;
}
