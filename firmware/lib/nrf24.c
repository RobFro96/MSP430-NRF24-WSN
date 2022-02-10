/**
 * MSP430 and NRF24L01 based WSN
 * @file term.c
 * @author Robert Fromm
 * @date 06.02.2022
 */

#include <msp430.h>
#include <stdint.h>

#include "pins.h"
#include "spia.h"
#include "isr.h"
#include "nrf24.h"

const uint8_t nrf24_default_profile[] = { NRF24_MASK_MAX_RT + NRF24_EN_CRC + NRF24_CRCO,  // R0: CONFIG
        0, // R1: EN_AA
        NRF24_ERX_P0, // R2: EN_RXADDR
        NRF24_AW * 3, // R3: SETUP_AW
        0, // R4: SETUP_RETR
        2, // R5: RF_CH
        NRF24_RF_DR_LOW + 3 * NRF24_RF_PWR // R6: RF_SETUP
};

uint8_t nrf24_rx_data[32];
nrf24_state_t current_state;

void nrf24_init() {
    p_nrf_cs_h();
    p_nrf_ce_l();

    nrf24_read_status();
    nrf24_read_status();
    nrf24_read_status();
    nrf24_flush();

    for (uint8_t reg = 0; reg < sizeof(nrf24_default_profile); reg++) {
        nrf24_write_register(reg, nrf24_default_profile[reg]);
    }

    nrf24_write_register(NRF24_RX_PW_P0, NRF24_PAYLOAD_SIZE);

    current_state = NRF24_STATE_SLEEP;
}

void nrf24_write_register_multibit(uint8_t addr, uint8_t len, const uint8_t *data) {
    p_nrf_cs_l();

    spia_send(NRF24_W_REGISTER + addr);

    while (len > 0) {
        spia_send(*data);
        data++;
        len--;
    }

    spia_wait_cs_disable();
    p_nrf_cs_h();
}

void nrf24_write_register(uint8_t addr, uint8_t val) {
    p_nrf_cs_l();

    spia_send(NRF24_W_REGISTER + addr);
    spia_send(val);

    spia_wait_cs_disable();
    p_nrf_cs_h();
}

uint8_t nrf24_read_register(uint8_t addr) {
    p_nrf_cs_l();

    spia_send(NRF24_R_REGISTER + addr);
    uint8_t val = spia_transmit(NRF24_NOP);

    p_nrf_cs_h();
    return val;
}

void nrf24_command(uint8_t cmd) {
    p_nrf_cs_l();

    spia_send(cmd);

    spia_wait_cs_disable();
    p_nrf_cs_h();
}

void nrf24_flush() {
    nrf24_command(NRF24_FLUSH_RX);
    nrf24_command(NRF24_FLUSH_TX);
    nrf24_write_register(NRF24_STATUS, 0xFF);
}

uint8_t nrf24_read_status_and_clear() {
    p_nrf_cs_l();

    uint8_t status = spia_transmit(NRF24_W_REGISTER + NRF24_STATUS);
    spia_send(0xFF);

    spia_wait_cs_disable();
    p_nrf_cs_h();

    return status;
}

void nrf24_enter_tx() {
    p_nrf_ce_l();
    nrf24_flush();
    nrf24_write_register(0, nrf24_default_profile[0] | NRF24_PWR_UP);
    switch (current_state) {
    case NRF24_STATE_SLEEP:
        isr_delay(50); // 1.5 ms
        break;
    case NRF24_STATE_TX:
        break;
    case NRF24_STATE_RX:
        p_delay_us(130); // 130 �s
        break;
    }
    current_state = NRF24_STATE_TX;
}

void nrf24_enter_rx() {
    p_nrf_ce_l();
    nrf24_flush();
    nrf24_write_register(0, nrf24_default_profile[0] | NRF24_PWR_UP | NRF24_PRIM_RX);
    switch (current_state) {
    case NRF24_STATE_SLEEP:
        isr_delay(50); // 1.5 ms
        break;
    case NRF24_STATE_TX:
        p_delay_us(130); // 130 �s
        break;
    case NRF24_STATE_RX:
        break;
    }
    current_state = NRF24_STATE_RX;
    p_nrf_ce_h();
}

void nrf24_enter_sleep() {
    p_nrf_ce_l();
    nrf24_flush();
    nrf24_write_register(0, nrf24_default_profile[0]);
    current_state = NRF24_STATE_SLEEP;
}

void nrf24_tx_data(uint8_t len, const uint8_t *data) {
    isr_flags &= ~ISR_NRF24_IRQ;
    nrf24_flush();

    // data to FIFO
    p_nrf_cs_l();
    spia_send(NRF24_W_TX_PAYLOAD);
    uint8_t remaining = NRF24_PAYLOAD_SIZE - len;
    while (len > 0) {
        spia_send(*data);
        data++;
        len--;
    }
    while (remaining > 0) {
        spia_send(0xFF);
        remaining--;
    }
    spia_wait_cs_disable();
    p_nrf_cs_h();

    p_nrf_ce_h();
    p_delay_us(10);
    p_nrf_ce_l();
}

uint8_t nrf24_wait_on_finished(uint16_t timeout) {
    if (timeout > 0) {
        isr_timeout(timeout);
    } else {
        isr_flags &= ~ISR_TIMER_DELAY;
    }

    while (1) {
        if (isr_flag_isset_with_clear(ISR_NRF24_IRQ) || !p_nrf_irq_val()) {
            uint8_t status = nrf24_read_status_and_clear();
            if (((current_state == NRF24_STATE_TX) && (status & NRF24_TX_DS))
                    || ((current_state == NRF24_STATE_RX) && (status & NRF24_RX_DR))) {
                return 1;
            } else {
                return 0;
            }
        } else if (isr_flag_isset_with_clear(ISR_TIMER_DELAY)) {
            return 0;
        } else {
            __low_power_mode_3();
        }
    }
}

void nrf24_rx_download() {
    p_nrf_cs_l();

    spia_send(NRF24_R_RX_PAYLOAD);
    uint8_t len = NRF24_PAYLOAD_SIZE;
    uint8_t *data = nrf24_rx_data;
    while (len > 0) {
        *data = spia_transmit(NRF24_NOP);
        data++;
        len--;
    }

    p_nrf_cs_h();
}
