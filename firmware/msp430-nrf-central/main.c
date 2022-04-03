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
#include "spib.h"
#include "nrf24.h"
#include "isr.h"
#include "uart.h"

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
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    p_setup();
    spib_init();
    uart_init();
    __enable_interrupt();

    for (uint8_t i = 0; i < sizeof(in_regs.led_dis); i++) {
        in_regs.led_dis[i] = 0;
    }
    in_regs.testing = 0;
    in_regs.status_led = 0;

    nrf24_init();
    nrf24_set_addr(NRF24_RX_ADDR_P0, 0);

    nrf24_enter_rx();
    while (1) {
        if (isr_flag_isset_with_clear(ISR_NRF24_IRQ) || !p_nrf_irq_val()) {
            uint8_t status = nrf24_read_status_and_clear();
            if (status & NRF24_RX_DR) {
                nrf24_rx_download(sizeof(out_regs), (uint8_t*) &out_regs);

                p_delay_ms(1); // wait because sensor is slower due to 1 MHz clock

                // Send ACK
                nrf24_enter_tx();
                nrf24_set_addr(NRF24_TX_ADDR, out_regs.addr);
                nrf24_tx_data(sizeof(in_regs), (uint8_t*) &in_regs);
                nrf24_wait_on_finished(327);

                nrf24_enter_rx();

                p_led_r_toggle();
                uart_send(UART_START_BYTE);
                uart_send(sizeof(out_regs));
                for (uint8_t i = 0; i < sizeof(out_regs); i++) {
                    uart_send(((uint8_t *) &out_regs)[i]);
                }
                p_led_r_toggle();
            }
            nrf24_flush();
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
