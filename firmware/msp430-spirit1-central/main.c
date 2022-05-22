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

typedef struct {
    uint8_t addr; // 00: device’s RF address A[7:0]
    uint8_t retries; // 01: count of current RF retries R[7:0]
    uint16_t vbat; // 02:03: 12-bit ADC reading of battery measurement N[11:0], reference R=0: 1.5 V, R=1: 2.5 V
    uint16_t temperature_internal; // 04:05: 12-bit ADC reading of internal temperature N[11:0]
    uint16_t si7021_humidity; // 06:07: Si7021 relative humidity raw reading N
    uint16_t si7021_temperature; // 08:09: Si7021 temperature raw reading N
    int16_t bmp180_temperature; // 0a:0b: BMP180 temperature reading in 0.1 degree Celsius
    int32_t bmp180_pressure; // 0c:0f: BMP180 pressure reading in Pascal
    uint8_t reed; // 10: reed switch state: R[7:0]=0x00 (magnetic, L), R=0xAA (released, H)
} out_regs_t;

typedef struct {
    uint8_t led_dis[8]; // 00:07: Bitwise array: enable LED on each node
    uint8_t testing; // 08: Id of sensor node in test mode
    uint8_t status_led; // 09: status LED state
} in_regs_t;

out_regs_t out_regs;
in_regs_t in_regs;

void update_rgb_led();

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

    for (uint8_t i = 0; i < sizeof(in_regs.led_dis); i++) {
        in_regs.led_dis[i] = 0;
    }
    in_regs.testing = 0;
    in_regs.status_led = 0;

    const uint8_t control_mask[] = CONTROL_MASK;

    spirit_wakeup();
    spirit_tx_set_ctrl(4, CONTROL_WORD);
    spirit_rx_set_ctrl(4, control_mask, CONTROL_WORD);
    spirit_rx_set_addr(MY_ADDRESS);
    spirit_rx_start();

    while (1) {
        if (isr_flag_isset_with_clear(ISR_SPIRIT_IRQ) || !p_spirit_irq_val()) {
            if (spirit_rx_is_msg_present()) {
                spirit_rx_download_info();
                spirit_rx_stop();

                memcpy(&out_regs, spirit_msg.data, sizeof(out_regs));

                // Send ACK
                spirit_tx_set_addr(out_regs.addr);
                spirit_tx_data(sizeof(in_regs), (uint8_t*) &in_regs);
                spirit_tx_wait_on_finished(10);

                spirit_rx_start();

                p_led_h();
                uart_send(UART_START_BYTE);
                uart_send(sizeof(out_regs));
                for (uint8_t i = 0; i < sizeof(out_regs); i++) {
                    uart_send(((uint8_t *) &out_regs)[i]);
                }
                p_led_l();
            }
        } else if (isr_flag_isset_with_clear(ISR_UART_IRQ)) {
            uint8_t len = uart_rx_buffer[UART_LEN];
            for (uint8_t i = 0; i < len; i++) {
                ((uint8_t *) &in_regs)[i] = uart_rx_buffer[UART_DATA + i];
            }
            update_rgb_led();
        } else {
            __low_power_mode_4();
        }
    }
}

void update_rgb_led() {
    if (in_regs.status_led & BIT0) {
        p_led_r_h();
    } else {
        p_led_r_l();
    }

    if (in_regs.status_led & BIT1) {
        p_led_g_h();
    } else {
        p_led_g_l();
    }

    if (in_regs.status_led & BIT2) {
        p_led_b_h();
    } else {
        p_led_b_l();
    }
}


