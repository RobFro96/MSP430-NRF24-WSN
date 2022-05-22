#ifndef SPIRIT1_SPIRIT_TX_H_
#define SPIRIT1_SPIRIT_TX_H_

void spirit_tx_set_power(uint8_t reg_value);
void spirit_tx_start_cw();
void spirit_tx_start_pn9();
void spirit_tx_stop_cw_pn9();
void spirit_tx_data(uint8_t len, const uint8_t *data);
void spirit_tx_custom_data_begin(uint8_t len);
void spirit_tx_custom_data_end();
uint8_t spirit_tx_wait_on_finished();
void spirit_tx_set_ctrl(uint8_t len, const uint8_t* ctrl);

/**
 * Set the address for package transmission
 * @param addr address to be transmitted
 */
inline void spirit_tx_set_addr(uint8_t addr) {
	spirit_write_register(PCKT_FLT_GOALS_SOURCE_ADDR_BASE, addr);
}

#endif /* SPIRIT1_SPIRIT_TX_H_ */
