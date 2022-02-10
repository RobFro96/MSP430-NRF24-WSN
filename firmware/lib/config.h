/**
 * MSP430 and NRF24L01 based WSN
 * @file config.h
 * @author Robert Fromm
 * @date 06.02.2022
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#if 0
#define PROG_ID 'R'
#else
#define PROG_ID 'T'
#endif

#define P_CLOCK_FREQ_MHZ 1  // DCO Calibration: frequency of MCLK/SMCLK

#define SPIA_PRESCALER 1 // Prescaler of the SPIA: SPI frequency = SMCLK / prescaler
#define SPIA_USE_FLAG_POLLING 0 // Use flag polling to delay the runtime [1] or use __delay_cycles() [0]

#define TERM_ENABLE 1

#define NRF24_PAYLOAD_SIZE (32)

#endif /* CONFIG_H_ */
