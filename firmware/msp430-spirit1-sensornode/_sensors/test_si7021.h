#ifndef TEST_SI7021_H_
#define TEST_SI7021_H_

void test_si7021_i2c_detect();
void test_si7021_measurement();
void test_si7021_measurement_function();
void test_si7021_print_results(uint8_t result, uint16_t raw_humidity, uint16_t raw_temperature);
void test_si7021_serial_number();

#endif /* TEST_SI7021_H_ */
