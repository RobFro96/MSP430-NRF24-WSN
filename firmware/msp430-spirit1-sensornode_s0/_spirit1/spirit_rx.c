/**
 * MSP430 and NRF24L01 based WSN
 * @file spirit_rx.c
 * @author Robert Fromm
 * @date 06.02.2022
 *
 * Functions controlling the receiving mode of the SPIRIT1.
 */

#include <msp430.h>
#include <stdint.h>

#include "isr.h"
#include "spirit.h"
#include "pins.h"
#include "spirit_rx.h"

spirit_msg_t spirit_msg;	//! last received message by calling spirit_rx()
spirit_msg_info_t spirit_msg_info;	//! extra information of the last received message

/**
 * Enter the RX mode for SPIRIT1 to listen for packages.
 */
void spirit_rx_start() {
	isr_flags &= ~ISR_SPIRIT_IRQ;
	spirit_enter_no_wait(SPIRIT_STATE_RX);
}

/**
 * Check whether a package was received by the SPIRIT1
 * @return 0 if no package was received
 */
uint8_t spirit_rx_is_msg_present() {
	// check interrupt flag: rx data ready
	if (!spirit_check_irq0(IRQ_STATUS0_RX_DATA_READY)) {
		return 0;
	}

	// read package length
	spirit_read_register_burst(RX_PCKT_LEN0_BASE, 1, &spirit_msg.len);
	spirit_msg.len -= SPIRIT_PCK_LEN_OFFSET;

	if (spirit_msg.len > SPIRIT_MAX_PAYLOAD_LEN) {
		return 0;
	}

	// read payload
	spirit_read_register_burst(0xff, spirit_msg.len, spirit_msg.data);

	return spirit_msg.len;
}

/**
 * Download package extra information:
 * - Link qualifiers: PQI, SQI, AGC, RSSI
 * - Package information: Control Bytes, Source, Destination Address
 */
void spirit_rx_download_info() {
	spirit_read_register_burst(LINK_QUALIF2_BASE, 4, &spirit_msg_info.pqi);
	spirit_read_register_burst(RX_CTRL_FIELD0_BASE, 6, &spirit_msg_info.ctrl[0]);
}

/**
 * Wait until a message is received or the timeout is expired.
 * Enter the Low-Power-Mode while waiting. The function spirit_wait_update() is after LPM wakeup.
 * The RX mode is not stopped in this function. Call spirit_rx_stop().
 *
 * @param timeout duration of the isr_timeout() function
 * @return length of the received message or 0
 */
uint8_t spirit_rx_timeout(uint16_t ms) {
    if (ms > 0) {
        isr_timeout(ms * P_CLOCK_ACLK);
	} else {
		isr_flags &= ~ISR_TIMER_DELAY;
	}

	while (1) {
		spirit_wait_updates();
		if (isr_flag_isset_with_clear(ISR_SPIRIT_IRQ) || !p_spirit_irq_val()) {
			if (spirit_rx_is_msg_present()) {
				return spirit_msg.len;
			}

		}
		else if (isr_flag_isset_with_clear(ISR_TIMER_DELAY)) {
			return 0;
		}
		else {
			__low_power_mode_3();
		}
	}
}

/**
 * Stop the package receiving.
 * Sending the SABORT command to the SPIRIT1.
 */
void spirit_rx_stop() {
	spirit_command(COMMAND_SABORT);
	spirit_current_state = SPIRIT_STATE_READY;
}

/**
 * Set Control Filtering settings.
 * For Control Filtering an additional mask can be used. If a mask bit is 0 the corresponding bit
 * is ignored while comparing. All 0s: no filtering on this control byte
 *
 * @param len number of control bytes
 * @param mask masks
 * @param ctrl control fields
 */
void spirit_rx_set_ctrl(uint8_t len, const uint8_t* mask, const uint8_t* ctrl) {
	spirit_write_register_burst(PCKT_FLT_GOALS_CONTROL0_MASK_BASE, len, mask);
	spirit_write_register_burst(PCKT_FLT_GOALS_CONTROL0_FIELD_BASE, len, ctrl);
}
