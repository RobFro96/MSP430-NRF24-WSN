/**
 * MSP430 and NRF24L01 based WSN
 * @file main.c
 * @author Robert Fromm
 * @date 06.02.2022
 */
#include <msp430.h>
#include <stdint.h>

#include "config.h"
#include "pins.h"
#include "spia.h"
#include "i2c.h"
#include "term.h"
#include "nrf24.h"
#include "test_nrf24.h"
#include "isr.h"
#include "si7021.h"
#include "test_si7021.h"
#include "sensor.h"
#include "test_bmp180.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    p_setup();
    spia_init();
    i2c_init();
    __enable_interrupt();
    term_wait_and_clear();

    nrf24_init();
    nrf24_set_addr(NRF24_RX_ADDR_P0, RF_ADDR_LSB);
    nrf24_set_addr(NRF24_TX_ADDR, 0);


    // TESTS
#if TERM_ENABLE
    test_nrf24_registers();
#if SI7021_ENABLE
    test_si7021_i2c_detect();
    test_si7021_serial_number();
#endif
#if BMP180_ENABLE
    test_bmp180_i2c_detect();
    test_bmp180_get_cal_param();
#endif
#endif

#if RF_ADDR_LSB == 0
    test_nrf24_rx_loop();
#else
    sensor_mainloop();
#endif
}
