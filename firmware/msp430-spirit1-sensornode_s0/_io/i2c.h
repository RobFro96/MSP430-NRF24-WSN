#ifndef I2C_H_
#define I2C_H_

void i2c_init();
uint8_t i2c_write(uint8_t address, const uint8_t* data, uint8_t len);
uint8_t i2c_read(uint8_t address, uint8_t* data, uint8_t len);

#endif /* I2C_H_ */
