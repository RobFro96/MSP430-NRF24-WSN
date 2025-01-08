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
#include "spib.h"
#include "uart.h"
#include "isr.h"
#include "spirit.h"

int16_t in_regs[8];
int16_t out_regs[17];

#define ADDR_OUT_REG_INDEX 0
#define RSSI_OUT_REG_INDEX 16


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	p_setup();
    BCSCTL3 |= LFXT1S_2;
	
	spib_init();
    uart_init();
	__enable_interrupt();

	// SPIRIT1 initialization
	spirit_reset();
    spirit_profile_apply(spirit_profile_basic_868mhz);
    spirit_profile_apply(spirit_profile_fsk_38k4);
    spirit_profile_apply(spirit_profile_pck);
	spirit_do_calibration();

    const uint8_t control_mask[] = CONTROL_MASK;

    spirit_wakeup();
    spirit_tx_set_ctrl(4, CONTROL_WORD);
    spirit_rx_set_ctrl(4, control_mask, CONTROL_WORD);
    spirit_rx_set_addr(MY_ADDRESS);
    spirit_rx_start();

    uart_send('\n');
    uart_send('\n');
    uart_send('\n');

    while (1) {
        if (isr_flag_isset_with_clear(ISR_SPIRIT_IRQ) || !p_spirit_irq_val()) {
            if (spirit_rx_is_msg_present()) {
                spirit_rx_download_info();
                spirit_rx_stop();

                memcpy(&out_regs, spirit_msg.data, sizeof(out_regs));
                out_regs[RSSI_OUT_REG_INDEX] = (spirit_msg_info.rssi >> 1) - 130;

                // Send ACK
                spirit_tx_set_addr(out_regs[ADDR_OUT_REG_INDEX]);
                spirit_tx_data(sizeof(in_regs), (uint8_t*) &in_regs);
                spirit_tx_wait_on_finished(20);

                spirit_rx_start();

                p_led_h();
                uart_send('\n');
                for (uint8_t i = 0; i < sizeof(out_regs) / 2; i++) {
                    uart_send_hex16(out_regs[i]);
                }
                uart_send('\n');
                p_led_l();
            }
        } else if (isr_flag_isset_with_clear(ISR_UART_IRQ)) {
            uart_convert_buffer_to_int16(in_regs);
        } else {
            __low_power_mode_4();
        }
    }
}



