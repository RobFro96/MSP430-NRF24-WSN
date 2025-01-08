#ifndef SPIRIT1_SPIRIT_RX_H_
#define SPIRIT1_SPIRIT_RX_H_

#define SPIRIT_MAX_PAYLOAD_LEN 92 //! maximum used payload length. Needed for size of the spirit_msg variable

typedef struct {
	uint8_t len;
	uint8_t data[SPIRIT_MAX_PAYLOAD_LEN];
} spirit_msg_t;

typedef struct {
	uint8_t pqi;
	uint8_t sqi;
	uint8_t agc;
	uint8_t rssi;
	uint8_t ctrl[4];
	uint8_t src_addr;
	uint8_t dest_addr;
} spirit_msg_info_t;

extern spirit_msg_t spirit_msg;
extern spirit_msg_info_t spirit_msg_info;

void spirit_rx_start();
uint8_t spirit_rx_is_msg_present();
void spirit_rx_download_info();
uint8_t spirit_rx_timeout(uint16_t timeout);
void spirit_rx_stop();
void spirit_rx_set_ctrl(uint8_t len, const uint8_t* mask, const uint8_t* ctrl);

/**
 * Set address for address filtering
 * @param addr receiver address
 */
inline void spirit_rx_set_addr(uint8_t addr) {
	spirit_write_register(PCKT_FLT_GOALS_TX_ADDR_BASE, addr);
}

/**
 * Set address for broadcast filtering
 * @param addr broadcast address
 */
inline void spirit_rx_set_boardcast(uint8_t addr) {
	spirit_write_register(PCKT_FLT_GOALS_BROADCAST_BASE, addr);
}

/**
 * Set address for multicast filtering
 * @param addr multicast address
 */
inline void spirit_rx_set_multicast(uint8_t addr) {
	spirit_write_register(PCKT_FLT_GOALS_MULTICAST_BASE, addr);
}

#endif /* SPIRIT1_SPIRIT_RX_H_ */

