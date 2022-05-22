/**
 * MSP430 and NRF24L01 based WSN
 * @file spirit_tx.c
 * @author Robert Fromm
 * @date 06.02.2022
 *
 * Functions to transmit a RF signal with the SPIRIT1
 */

#include <msp430.h>
#include <stdint.h>

#include "isr.h"
#include "spirit.h"
#include "spirit_tx.h"

/**
 * Set the RF transmission power of the SPIRIT1
 * reg_value is the values of the corresponding register.
 * See datasheet, page 52, table 28
 * 1 ... 11 dBm (max.)
 * 30 ... 0 dBm
 * 42 ... -6 dBm
 * 90 ... -34 dBm (min.)
 * half of dB per register value +1
 *
 * @param reg_value register value, see above
 */
void spirit_tx_set_power(uint8_t reg_value) {
	spirit_write_register(PA_POWER8_BASE, reg_value);
}

/**
 * Starting a continuous wave transmission with the SPIRIT1 module.
 * As TXSOURCE PN9 is used to achieve a basically infinite transmission time.
 * Stop transmission by calling spirit_stop_cw_pn9()
 * The SPIRIT1 must to be in WAKEUP state.
 */
void spirit_tx_start_cw() {
	spirit_write_register(MOD0_BASE, BIT7); // CW
	spirit_write_register(PCKTCTRL1_BASE,
			(SPIRIT_PCK_CRC_MODE << 5) + (SPIRIT_PCK_WHIT_EN << 4) + BIT3 + BIT2); // TXSOURCE = PN9
	spirit_enter(SPIRIT_STATE_TX);
}

/**
 * Transmitting a PN9 signal with the current modulation setting.
 * The SPIRIT1 must to be in WAKEUP state.
 */
void spirit_tx_start_pn9() {
	spirit_write_register(PCKTCTRL1_BASE,
			(SPIRIT_PCK_CRC_MODE << 5) + (SPIRIT_PCK_WHIT_EN << 4) + BIT3 + BIT2); // TXSOURCE = PN9
	spirit_enter(SPIRIT_STATE_TX);
}

/**
 * Removing PN9 from the TXSOURCE and enter ready state.
 * This function is used to stop a CW or a PN9 transmission.
 * Registers MOD0 and PCKCTRL1 are restored to default profile's values. Profile reload might be needed.
 */
void spirit_tx_stop_cw_pn9() {
	spirit_write_register(MOD0_BASE, (SPIRIT_FSK_38K4_BT_SEL << 6) + (SPIRIT_FSK_38K4_MOD_TYPE << 4) // Modulation
			+ SPIRIT_FSK_38K4_DATARATE_E);
	spirit_write_register(PCKTCTRL1_BASE, (SPIRIT_PCK_CRC_MODE << 5) + (SPIRIT_PCK_WHIT_EN << 4)); // TXSOURCE = normal
	spirit_command(COMMAND_SABORT);
	spirit_enter(SPIRIT_STATE_READY);

	// clear all IFGs
	spirit_check_irq0(0);
}

/**
 * Sending the given message with the current settings.
 * The SPIRIT1 must to be in WAKEUP state.
 *
 * @param len number of data bytes to be sent.
 * @param data data to be sent.
 */
void spirit_tx_data(uint8_t len, const uint8_t *data) {
	isr_flags &= ~ISR_SPIRIT_IRQ;
	spirit_command(COMMAND_FLUSHTXFIFO);
	spirit_write_register(PCKTLEN0_BASE, len + SPIRIT_PCK_LEN_OFFSET);
	spirit_write_register_burst(0xff, len, data);
	spirit_enter(SPIRIT_STATE_TX);
}

/**
 * Starting a transmission. Using the correspoding SPI module to fill the FIFO of the SPIRIT1.
 * Use the function spirit_tx_custom_data_end() to start the transmission.
 * The SPIRIT1 must to be in WAKEUP state.
 *
 * @param len payload length of message (including control bits)
 */
void spirit_tx_custom_data_begin(uint8_t len) {
	isr_flags &= ~ISR_SPIRIT_IRQ;
	spirit_command(COMMAND_FLUSHTXFIFO);
	spirit_write_register(PCKTLEN0_BASE, len);

	spirit_cs_select();
	spirit_spi_send(SPIRIT_SPI_ADDRESS + SPIRIT_SPI_WRITE);
	spirit_spi_send(0xff);
}

/**
 * Corresponding function to spirit_tx_custom_data_begin().
 * Should be executed after the FIFO was filled manually.
 * Starting the tranmission of the packet.
 */
void spirit_tx_custom_data_end() {
	spirit_cs_disable();
	spirit_enter(SPIRIT_STATE_TX);
}

/**
 * Wait until a message is sent or the timeout is expired.
 * Enter the Low-Power-Mode while waiting. The function spirit_wait_update() is after LPM wakeup.
 *
 * @param timeout duration of the isr_timeout() function
 * @return 1 if the message was sent successfully.
 */
uint8_t spirit_tx_wait_on_finished(uint16_t ms) {
	if (spirit_current_state != SPIRIT_STATE_TX)
		return 0;

    if (ms > 0) {
        isr_timeout(ms * P_CLOCK_ACLK);
	} else {
		isr_flags &= ~ISR_TIMER_DELAY;
	}

	while (1) {
		spirit_wait_updates();
		if (isr_flag_isset_with_clear(ISR_SPIRIT_IRQ) || !p_spirit_irq_val()) {
			spirit_enter(SPIRIT_STATE_READY);
			uint8_t tx_irq = spirit_check_irq0(IRQ_STATUS0_TX_DATA_SENT);
			return tx_irq ? 1 : 0;
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
 * Set the control bytes for package transmission.
 *
 * @param len number of control byte to write
 * @param ctrl control bytes
 */
void spirit_tx_set_ctrl(uint8_t len, const uint8_t* ctrl) {
	spirit_cs_select();

	spirit_spi_send(SPIRIT_SPI_ADDRESS + SPIRIT_SPI_WRITE);
	spirit_spi_send(TX_CTRL_FIELD3_BASE);

	// send ctrl bytes in reversed order
	while (len > 0) {
		len--;
		spirit_spi_send(ctrl[len]);
	}

	spirit_cs_disable();
}
