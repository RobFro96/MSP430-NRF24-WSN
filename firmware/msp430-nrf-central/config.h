/**
 * MSP430 and NRF24L01 based WSN
 * @file config.h
 * @author Robert Fromm
 * @date 06.02.2022
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#define NRF24_CHANNEL (90)
#define TERM_ENABLE 1
#define RF_ADDR_0 'R'
#define RF_ADDR_1 'o'
#define RF_ADDR_2 'F'
#define RF_ADDR_3 'r'

#define P_CLOCK_FREQ_MHZ 8  // DCO Calibration: frequency of MCLK/SMCLK
#define P_CLOCK_ACLK 32

#define SPIB_PRESCALER 1 // Prescaler of the SPIA: SPI frequency = SMCLK / prescaler
#define SPIB_USE_FLAG_POLLING 0 // Use flag polling to delay the runtime [1] or use __delay_cycles() [0]

#define NRF24_SPI 'B'
#define NRF24_PAYLOAD_SIZE (32) // NRF24 fixed payload size

#endif /* CONFIG_H_ */
