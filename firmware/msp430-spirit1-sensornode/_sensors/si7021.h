#ifndef SI7021_H_
#define SI7021_H_

#define SI7021_I2C_ADDR (0x40) //! 7bit I2C address of SI7021
#define SI7021_TRIGGER_TEMP_HOLD (0xE3) //! Command: temperature measurement HOLD mode
#define SI7021_TRIGGER_HUMI_HOLD (0xE5) //! Command: humidity measurement HOLD mode
#define SI7021_TRIGGER_TEMP_NOHOLD (0xF3) //! Command: temperature measurement NOHOLD mode
#define SI7021_TRIGGER_HUMI_NOHOLD (0xF5) //! Command: humidity measurement NOHOLD mode

uint8_t si7021_start_temperature();
uint8_t si7021_start_humidity();
void si7021_temperature_delay();
void si7021_humidity_delay();
uint8_t si7021_read_result(uint16_t* value);
uint8_t si7021_measurement(uint16_t* temperature, uint16_t* humidity);
int16_t si7021_convert_temperature_2decimals();
uint16_t si7021_convert_humidity_2decimals();

#endif /* SI7021_H_ */
