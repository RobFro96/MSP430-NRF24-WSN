#ifndef NRF24_H_
#define NRF24_H_
// source: https://github.com/spirilis/msprf24/blob/master/nRF24L01.h

/* Register Map */
#define NRF24_CONFIG      0x00
#define NRF24_EN_AA       0x01
#define NRF24_EN_RXADDR   0x02
#define NRF24_SETUP_AW    0x03
#define NRF24_SETUP_RETR  0x04
#define NRF24_RF_CH       0x05
#define NRF24_RF_SETUP    0x06
#define NRF24_STATUS      0x07
#define NRF24_OBSERVE_TX  0x08
#define NRF24_CD          0x09
#define NRF24_RPD         0x09
#define NRF24_RX_ADDR_P0  0x0A
#define NRF24_RX_ADDR_P1  0x0B
#define NRF24_RX_ADDR_P2  0x0C
#define NRF24_RX_ADDR_P3  0x0D
#define NRF24_RX_ADDR_P4  0x0E
#define NRF24_RX_ADDR_P5  0x0F
#define NRF24_TX_ADDR     0x10
#define NRF24_RX_PW_P0    0x11
#define NRF24_RX_PW_P1    0x12
#define NRF24_RX_PW_P2    0x13
#define NRF24_RX_PW_P3    0x14
#define NRF24_RX_PW_P4    0x15
#define NRF24_RX_PW_P5    0x16
#define NRF24_FIFO_STATUS 0x17
#define NRF24_DYNPD       0x1C
#define NRF24_FEATURE     0x1D

/* Register Bits */
#define NRF24_MASK_RX_DR  BIT6
#define NRF24_MASK_TX_DS  BIT5
#define NRF24_MASK_MAX_RT BIT4
#define NRF24_EN_CRC      BIT3
#define NRF24_CRCO        BIT2
#define NRF24_PWR_UP      BIT1
#define NRF24_PRIM_RX     BIT0
#define NRF24_ENAA_P5     BIT5
#define NRF24_ENAA_P4     BIT4
#define NRF24_ENAA_P3     BIT3
#define NRF24_ENAA_P2     BIT2
#define NRF24_ENAA_P1     BIT1
#define NRF24_ENAA_P0     BIT0
#define NRF24_ERX_P5      BIT5
#define NRF24_ERX_P4      BIT4
#define NRF24_ERX_P3      BIT3
#define NRF24_ERX_P2      BIT2
#define NRF24_ERX_P1      BIT1
#define NRF24_ERX_P0      BIT0
#define NRF24_AW          BIT0
#define NRF24_ARD         BIT4
#define NRF24_ARC         BIT0
#define NRF24_PLL_LOCK    BIT4
#define NRF24_CONT_WAVE   BIT7
#define NRF24_RF_DR       BIT3
#define NRF24_RF_DR_LOW   BIT5
#define NRF24_RF_DR_HIGH  BIT3
#define NRF24_RF_PWR      BIT1
#define NRF24_LNA_HCURR   BIT0
#define NRF24_RX_DR       BIT6
#define NRF24_TX_DS       BIT5
#define NRF24_MAX_RT      BIT4
#define NRF24_RX_P_NO     BIT1
#define NRF24_TX_FULL     BIT0
#define NRF24_PLOS_CNT    BIT4
#define NRF24_ARC_CNT     BIT0
#define NRF24_TX_REUSE    BIT6
#define NRF24_FIFO_FULL   BIT5
#define NRF24_TX_EMPTY    BIT4
#define NRF24_RX_FULL     BIT1
#define NRF24_RX_EMPTY    BIT0
#define NRF24_EN_DPL      BIT2
#define NRF24_EN_ACK_PAY  BIT1
#define NRF24_EN_DYN_ACK  BIT0

/* Instructions */
#define NRF24_R_REGISTER    0x00
#define NRF24_W_REGISTER    0x20
#define NRF24_REGISTER_MASK 0x1F
#define NRF24_R_RX_PAYLOAD  0x61
#define NRF24_W_TX_PAYLOAD  0xA0
#define NRF24_FLUSH_TX      0xE1
#define NRF24_FLUSH_RX      0xE2
#define NRF24_REUSE_TX_PL   0xE3
#define NRF24_R_RX_PL_WID   0x60
#define NRF24_W_ACK_PAYLOAD 0xA8
#define NRF24_W_TX_PAYLOAD_NOACK 0xB0
#define NRF24_NOP 0xFF

#if NRF24_SPI == 'A'
#include "spia.h"
#define nrf24_spi_transmit(data_out) (spia_transmit(data_out))
#define nrf24_spi_send(data_out) (spia_send(data_out))
#define nrf24_spi_wait_cs_disable() (spia_wait_cs_disable())
#elif NRF24_SPI == 'B'
#include "spib.h"
#define nrf24_spi_transmit(data_out) (spib_transmit(data_out))
#define nrf24_spi_send(data_out) (spib_send(data_out))
#define nrf24_spi_wait_cs_disable() (spib_wait_cs_disable())
#endif

typedef struct {
    uint8_t reg;
    uint8_t val;
} nrf24_reg_val_pair_t;

typedef enum {
    NRF24_STATE_SLEEP = 0, NRF24_STATE_TX, NRF24_STATE_RX
} nrf24_state_t;

extern const nrf24_reg_val_pair_t nrf24_default_profile[];
extern const uint8_t nrf24_default_profile_count;

void nrf24_init();
void nrf24_write_register_multibit(uint8_t addr, uint8_t len, const uint8_t *data);
void nrf24_write_register(uint8_t addr, uint8_t val);
uint8_t nrf24_read_register(uint8_t addr);
void nrf24_command(uint8_t cmd);
void nrf24_flush();
uint8_t nrf24_read_status_and_clear();
void nrf24_enter_tx();
void nrf24_enter_rx();
void nrf24_enter_sleep();
void nrf24_tx_data(uint8_t len, const uint8_t *data);
uint8_t nrf24_wait_on_finished(uint16_t timeout);
void nrf24_rx_download(uint8_t len, uint8_t *data);
void nrf24_set_addr(uint8_t reg, uint8_t lsb);

inline uint8_t nrf24_read_status() {
    return nrf24_read_register(NRF24_NOP);
}


#endif /* NRF24_H_ */
