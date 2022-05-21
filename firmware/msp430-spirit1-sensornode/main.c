/**
 * MSP430 and NRF24L01 based WSN
 * @file config.h
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
#include "isr.h"
#include "spirit.h"
#include "test_spirit.h"
#include "si7021.h"
#include "test_si7021.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	p_setup();
    BCSCTL3 |= LFXT1S_2;
	
	spia_init();
	i2c_init();
	__enable_interrupt();
	term_wait_and_clear();

	// SPIRIT1 initialization
	spirit_reset();
    spirit_profile_apply(spirit_profile_basic_868mhz);
    spirit_profile_apply(spirit_profile_fsk_38k4);
    spirit_profile_apply(spirit_profile_pck);
	spirit_do_calibration();
	term_log("SPRIT1 initialized.");

	// Tests
#if TERM_ENABLE
	test_spirit_read_burst();
	test_spirit_irq_toggle();
	test_spirit_sleep_wakeup();
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
    test_spirit_rx_loop_ack();
#else
    while (1) {
        test_spirit_tx_rx_ack();
        isr_delay_ms(1000);
    }
#endif
}

