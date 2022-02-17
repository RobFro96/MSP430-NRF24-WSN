/**
 * MSP430 and NRF24L01 based WSN
 * @file config.h
 * @author Robert Fromm
 * @date 06.02.2022
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#define TERM_ENABLE 1
#define RF_ADDR_0 'R'
#define RF_ADDR_1 'o'
#define RF_ADDR_2 'F'
#define RF_ADDR_3 'r'
#define RF_ADDR_LSB 1

// SENSORS
#define INTERNAL_TEMPERATURE_ENABLE 1
#define SI7021_ENABLE 1
#define BMP180_ENABLE 1
#define REED_EN 1

#define P_CLOCK_FREQ_MHZ 1  // DCO Calibration: frequency of MCLK/SMCLK
#define P_CLOCK_ACLK 10 // ACLK clock frequency in kHz

#define SPIA_PRESCALER 1 // Prescaler of the SPIA: SPI frequency = SMCLK / prescaler
#define SPIA_USE_FLAG_POLLING 0 // Use flag polling to delay the runtime [1] or use __delay_cycles() [0]

#define NRF24_SPI 'A'
#define NRF24_PAYLOAD_SIZE (32) // NRF24 fixed payload size

// I2C Settings
// Prescaler of the I2C: I2C frequency = SMCLK / prescaler
#if P_CLOCK_FREQ_MHZ == 1
#define I2C_PRESCALER 3 // 1 MHz / 3 = 333 kHz
#elif P_CLOCK_FREQ_MHZ == 8
#define I2C_PRESCALER 20 // 8 MHz / 20 = 400 kHz
#endif

// Si7021 Settings
// Two Timing configurations, _LONG=1 ensures compability with HTU21D
#define SI7021_TIMINGS_LONG 0
// 0: 12 ms RH, 11 ms T, 1: 16 ms RH, 50 ms T

// BMP180
#define BMP180_OSS (3) // Pressure measurement oversampling 0...3

#define MAX_RETRIES (5)
#define MIN_RETRIES (2)
#define RX_INTERVAL (20) // ms
#define RETRY_DELAY (100) // ms

#endif /* CONFIG_H_ */
