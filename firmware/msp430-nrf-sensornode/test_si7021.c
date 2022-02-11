/**
 * THE WAKEUP RECEIVER PROJECT
 * @file test_si7021.c
 * @author Robert Fromm
 * @date 11.02.2022
 *
 * Test functions for Si7021 I2C temperature sensors
 */

#include <msp430.h>
#include <stdint.h>

#include "config.h"
#include "isr.h"
#include "i2c.h"
#include "term.h"
#include "si7021.h"
#include "test_si7021.h"

/**
 * Test Si7021 is responding on I2C address 0x40
 */
void test_si7021_i2c_detect() {
	term_log("\e[1mtest_si7021_i2c_detect()\e[0m");
	uint8_t data[2];
	uint8_t result = i2c_read(SI7021_I2C_ADDR, data, 1);

	term_log_begin();
	term_print("I2C read on address 0x40: ");
	term_test_result(!result);
	term_end();
}

/**
 * Performe a measurement of temperature and humidity.
 * Show details: I2C results and raw data
 */
void test_si7021_measurement() {
    term_log("\e[1mtest_si7021_measurement()\e[0m");

    uint16_t raw_temperature, raw_humidity;

    uint8_t status1 = si7021_start_humidity();
    si7021_humidity_delay();
    uint8_t status2 = si7021_read_result(&raw_humidity);

    uint8_t status3 = si7021_start_temperature();
    si7021_temperature_delay();
    uint8_t status4 = si7021_read_result(&raw_temperature);

    term_log_begin();
    term_print("humidity W:");
    term_test_result(!status1);
    term_print("  R:");
    term_test_result(!status2);

    term_print("  temperature W:");
    term_test_result(!status3);
    term_print("  R:");
    term_test_result(!status4);
    term_end();

    term_log_begin();
    term_print("raw data RH=");
    term_hex(raw_humidity, 4);
    term_print("  T=");
    term_hex(raw_temperature, 4);
    term_end();

    term_log_begin();
    term_print("converted RH=");
    term_decimal(si7021_convert_humidity_2decimals(raw_humidity), 5, 2);
    term_print("%  T=");
    term_signed_decimal(si7021_convert_temperature_2decimals(raw_temperature), 5, 2);
    term_print("^C");
    term_end();
}

/**
 * Test the si7021_measurement() function. Show converted values.
 */
void test_si7021_measurement_function() {
    term_log("\e[1mtest_si7021_measurement_function()\e[0m");

    uint16_t raw_temperature, raw_humidity;
    uint8_t result = si7021_measurement(&raw_temperature, &raw_humidity);

    term_log_begin();
    term_print("success? ");
    term_test_result(!result);
    term_print("  RH=");
    term_decimal(si7021_convert_humidity_2decimals(raw_humidity), 5, 2);
    term_print("%  T=");
    term_signed_decimal(si7021_convert_temperature_2decimals(raw_temperature), 5, 2);
    term_print("^C");
    term_end();
}

/**
 * Read and display Si7021 serial number and firmware version.
 * Check values from datasheet, to ensure genuine Si7021
 */
void test_si7021_serial_number() {
	term_log("\e[1mtest_si7021_serial_number()\e[0m");

	const uint8_t req1[2] = { 0xfa, 0x0f };
	i2c_write(SI7021_I2C_ADDR, req1, 2);

	uint8_t res1[8];
	i2c_read(SI7021_I2C_ADDR, res1, 8);

	const uint8_t req2[2] = { 0xfc, 0xc9 };
	i2c_write(SI7021_I2C_ADDR, req2, 2);

	uint8_t res2[8];
	i2c_read(SI7021_I2C_ADDR, res2, 8);

	const uint8_t req3[2] = { 0x84, 0xb8 };
	i2c_write(SI7021_I2C_ADDR, req3, 2);

	uint8_t res3;
	i2c_read(SI7021_I2C_ADDR, &res3, 1);

	term_log_begin();
	term_print("S/N: ");
	term_hex(res1[0], 2);
	term_hex(res1[2], 2);
	term_hex(res1[4], 2);
	term_hex(res1[6], 2);
	term_hex(res2[0], 2);
	term_hex(res2[2], 2);
	term_hex(res2[4], 2);
	term_hex(res2[6], 2);

	term_print("  is Si7021? ");
	term_test_result(res2[0] == 0x15);
	term_end();

	term_log_begin();
	term_print("firmware: 0x");
	term_hex(res3, 2);
	term_print("  V2.0? ");
	term_test_result(res3 == 0x20);
	term_end();
}
