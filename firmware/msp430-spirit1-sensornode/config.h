/**
 * MSP430 and NRF24L01 based WSN
 * @file config.h
 * @author Robert Fromm
 * @date 06.02.2022
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "spirit_profiles.h"

#define CONTROL_WORD "RoFr"
#define CONTROL_MASK {0xFF, 0xFF, 0xFF, 0xFF}
#define MY_ADDRESS 4

#if MY_ADDRESS == 1
// [1] Testing
#define TERM_ENABLE 1
#define INTERNAL_TEMPERATURE_INDEX 0 // 3
#define SI7021_INDEX 3 // 3+4
#define DIGITAL_IN_INDEX 5 // 5
#define SLEEP_DEFAULT (60) // secs
#define OUT_PAYLOAD_LEN (6) // number of words

#elif MY_ADDRESS == 2
// [2] Weather Station
#define TERM_ENABLE 0
#define INTERNAL_TEMPERATURE_INDEX 0 // 3
#define SI7021_INDEX 3 // 3+4
#define DIGITAL_IN_INDEX 0 // 5
#define SLEEP_DEFAULT (120) // sec
#define OUT_PAYLOAD_LEN (5) // Number of words

#elif MY_ADDRESS == 4
// [4] Blume
#define TERM_ENABLE 0
#define INTERNAL_TEMPERATURE_INDEX 0 // 3
#define SI7021_INDEX 3 // 3+4
#define DIGITAL_IN_INDEX 0 // 5
#define RESISTOR_DIV_INDEX 5 // 5
#define SLEEP_DEFAULT (120) // sec
#define OUT_PAYLOAD_LEN (6) // Number of words
#endif

#define P_CLOCK_FREQ_MHZ 1  // DCO Calibration: frequency of MCLK/SMCLK
#define P_CLOCK_ACLK 10 // ACLK clock frequency in kHz

#define SPIA_PRESCALER 1 // Prescaler of the SPIA: SPI frequency = SMCLK / prescaler
#define SPIA_USE_FLAG_POLLING 0 // Use flag polling to delay the runtime [1] or use __delay_cycles() [0]
#define SPIB_PRESCALER 1 // Prescaler of the SPIA: SPI frequency = SMCLK / prescaler
#define SPIB_USE_FLAG_POLLING 0 // Use flag polling to delay the runtime [1] or use __delay_cycles() [0]

// I2C Settings
// Prescaler of the I2C: I2C frequency = SMCLK / prescaler
#if P_CLOCK_FREQ_MHZ == 1
#define I2C_PRESCALER 3 // 1 MHz / 3 = 333 kHz
#elif P_CLOCK_FREQ_MHZ == 8
#define I2C_PRESCALER 20 // 8 MHz / 20 = 400 kHz
#endif

// Si7021 Settings
// Two Timing configurations, _LONG=1 ensures compability with HTU21D
#define SI7021_TIMINGS_LONG 1
// 0: 12 ms RH, 11 ms T, 1: 16 ms RH, 50 ms T

// BMP180
#define BMP180_OSS (3) // Pressure measurement oversampling 0...3

#define MAX_RETRIES (5)
#define MIN_RETRIES (2)
#define RX_INTERVAL (20) // ms
#define RETRY_DELAY (100) // ms

#define SLEEP_TESTING (1) // sec

// SPIRIT1 Settings
#define SPIRIT_SPI 'A' // 'A': SPIA, 'B': SPIB
#define SPIRIT_ENTER_WDT_ENABLE 1 // 1: Use WDT inside the spirit_enter function, reset is generated if entering takes to long
#define SPIRIT_ENTER_WDT_INTERVAL (WDTIS0) // WDTSSEL and WDTISx bits: WDTIS0 = SMCLK/8192 = 1024 µs @ 8 MHz
#define SPIRIT_ULP_STATE (SPIRIT_STATE_STANDBY) // Default ULP state: SPIRIT_STATE_STANDBY or _SLEEP
#define SPIRIT_AUTO_VCO 1   // Reset to automatic VCO calibration after manual calibration

// SPIRIT1 Package Settings
#define SPIRIT_PCK_CHNUM 0 // 0...255: Channel Number
#define SPIRIT_PCK_CRC_MODE 1 // 0: No CRC, 1: 0x07, 2: 0x8005, 3: 0x1021, 4: 0x864CBF
#define SPIRIT_PCK_WHIT_EN 1 // 1: Enable Data Whitening
#define SPIRIT_PCK_SYNC_LEN 3 // 0...3: -> 1...4 Sync Bytes
#define SPIRIT_PCK_PREAMBLE_LEN 3 // 0...31 -> 1...32 Preamble Length
#define SPIRIT_PCK_ADDR_LEN 1 // 1: Send Receiver Address
#define SPIRIT_PCK_CTRL_LEN 4 // 0...4: Control Bytes
#define SPIRIT_PCK_SYNC1 0x88 // Sync Bytes
#define SPIRIT_PCK_SYNC2 0x88
#define SPIRIT_PCK_SYNC3 0x88
#define SPIRIT_PCK_SYNC4 0x88
#define SPIRIT_PCK_LEN_OFFSET (SPIRIT_PCK_ADDR_LEN + SPIRIT_PCK_CTRL_LEN) // Package Length vs. Payload Length because of additional bytes of Address and Control Fields

// Package Filtering
#define SPIRIT_PCK_CRC_FILTERING 1 // 1: Enable Package Filtering by CRC
#define SPIRIT_PCK_CONTROL_FILTERING 1 // 1: Match the Control Field registers
#define SPIRIT_PCK_ADDRESS_FILTERING 1 // 1: Match the Address Filed
#define SPIRIT_PCK_MULTICAST_FILTERING 0 // 1: Packages with matching MULTICAST Address are accepted
#define SPIRIT_PCK_BROADCAST_FILTERING 0 // 1: Packages with matching BROADCAST Address are accepted

inline void spirit_wait_updates() {
}

#endif /* CONFIG_H_ */
