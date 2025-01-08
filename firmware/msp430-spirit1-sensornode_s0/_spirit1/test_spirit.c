/**
 * MSP430 and NRF24L01 based WSN
 * @file test_spirit.c
 * @author Robert Fromm
 * @date 06.02.2022
 *
 * This module contains all test programs for the SPIRIT1 RF module,
 * that were used while programming. Test results are printed over terminal.
 */


#include <msp430.h>
#include <stdint.h>

#include "pins.h"
#include "isr.h"
#include "term.h"
#include "spirit_regs.h"
#include "spirit.h"
#include "test_spirit.h"

#include "isr.h"

/**
 * Test for spirit_read_register_burst() function.
 * Comparing the status bits with 0b0101001000000111.
 * And reading PARTNUM and VERSION registers and comparing their values with defaults.
 */
void test_spirit_read_burst() {
	term_log("\e[1mtest_spirit_read_burst()\e[0m");

	uint8_t data[2];
	spirit_read_register_burst(DEVICE_INFO1_PARTNUM, 2, data);
	uint16_t status = spirit_get_status();

	term_log_begin();
	term_print("status = 0b");
	term_binary(status, 16);
	term_print(" ");
	term_test_result(status == 0b0101001000000111);
	term_end();

	term_log_begin();
	term_print("PARTNUM = 0x");
	term_hex(data[0], 2);
	term_print(", VERSION = 0x");
	term_hex(data[1], 2);
	term_print(" ");
	term_test_result(data[0] == 0x01 && data[1] == 0x30);
	term_end();
}

/**
 * Setting the GPIO0 pin to VDD and reading the MSP430 PxIN value
 * Setting the GPIO0 pin to GND and waiting for a MSP430 interrupt.
 */
void test_spirit_irq_toggle() {
	term_log("\e[1mtest_spirit_irq_toggle()\e[0m");

	spirit_write_register(GPIO0_CONF_BASE, CONF_GPIO_OUT_VDD + CONF_GPIO_MODE_DIG_OUTL);

	term_log_begin();
	term_print("setting irq pin to H: ");
	term_test_result(p_spirit_irq_val());
	term_end();

	isr_flags = 0;
	spirit_write_register(GPIO0_CONF_BASE, CONF_GPIO_OUT_GND + CONF_GPIO_MODE_DIG_OUTL);

	term_log_begin();
	term_print("interrupt test with low power mode: ");

	while (!isr_flag_isset_with_clear(ISR_SPIRIT_IRQ)) {
		__low_power_mode_3();
	}

	term_test_result(1);
	term_end();

	// restore default
	spirit_write_register(GPIO0_CONF_BASE, CONF_GPIO_OUT_nIRQ + CONF_GPIO_MODE_DIG_OUTL);
}

/**
 * Setting SPIRIT1 to sleep mode.
 * Uncomment line //__low_power_mode_3(); for current measurement
 * Setting SPIRIT1 to ready mode.
 */
void test_spirit_sleep_wakeup() {
	term_log("\e[1mtest_spirit_sleep_wakeup()\e[0m");

	spirit_sleep();

	term_log_begin();
	term_print("sleeping: ");
	term_test_result(1);
	term_end();

	//__low_power_mode_3();  // uncomment for current measurement, program hangs up here

	spirit_wakeup();

	term_log_begin();
	term_print("wakeup: ");
	term_test_result(1);
	term_end();
}

/**
 * Transmitting a CW signal for 10 sec.
 * Using the LED to indicate the active transmission.
 */
void test_spirit_cw() {
	term_log("\e[1mtest_spirit_cw()\e[0m");

	spirit_tx_start_cw();
	p_led_h();
	term_log("Transmitting CW for 10 sec.");

	p_delay_ms(10000UL);
	p_led_l();

	spirit_tx_stop_cw_pn9();

	term_log("CW transmission stopped.");
}

/**
 * Transmitting a PN9 signal with the current modulation settings for 10 sec.
 * Using the LED to indicate the active transmission.
 */
void test_spirit_pn9() {
	term_log("\e[1mtest_spirit_pn9()\e[0m");

	spirit_tx_start_pn9();
	p_led_h();
	term_log("Transmitting PN9 for 10 sec.");

	p_delay_ms(10000UL);
	p_led_l();

	spirit_tx_stop_cw_pn9();

	term_log("PN9 transmission stopped.");
}

static const uint8_t regs[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
		0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x1A,
		0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x30, 0x31,
		0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40,
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
		0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
		0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x90, 0x91, 0x92, 0x93, 0x9E, 0x9F, 0xA0, 0xA1, 0xA4,
		0xA5, 0xA6, 0xA7, 0xB4, 0xE5 }; /*! All register addresses to be printed in
 test_spirit_print_registers() */

#define regs_len 106	//! Number of registers to be printed in test_spirit_print_registers()

/**
 * Print all selected register of the SPIRIT1 module. Using the constant regs[] and regs_len.
 * First all registers are read and second all registers are printed.
 */
void test_spirit_print_registers() {
	term_log("\e[1mtest_spirit_print_registers()\e[0m");
	uint8_t vals[regs_len];

	for (uint8_t i = 0; i < regs_len; i++) {
		spirit_read_register_burst(regs[i], 1, &(vals[i]));
	}

	for (uint8_t i = 0; i < regs_len; i++) {
		term_log_begin();
		term_print("0x");
		term_hex(regs[i], 2);
		term_print(": 0x");
		term_hex(vals[i], 2);
		term_end();
	}
}

/**
 * Print the status register of the SPIRIT1 and the state code.
 */
void test_spirit_print_status() {
	uint16_t status = spirit_get_status();

	term_log_begin();
	term_print("STATUS = ");
	term_binary(status, 16);
	term_end();

	term_log_begin();
	term_print("STATE[7:1] = 0x");
	term_hex((status & 0xFF) >> 1, 2);
	term_end();
}

/**
 * Sending a message in the data transfer mode.
 * Waiting for a IRQ signal in LPM. Checking signal source and IRQ pin returning to H after clearing
 * the interrupt flag.
 */
void test_spirit_tx_hello() {
	term_log("\e[1mtest_spirit_data_tx_hello()\e[0m");
	term_log("Sending message to SPIRIT1");

	spirit_wakeup();
	spirit_tx_data(5, "Hello");
	p_led_h();

	uint8_t success = spirit_tx_wait_on_finished(1638); // 50 ms

	p_led_l();
	term_log_begin();
	term_print("Tranmission successfull ");
	term_test_result(success);
	term_end();

	term_log_begin();
	term_print("IRQ_pin return to H? ");
	term_test_result(p_spirit_irq_val());
	term_end();

	spirit_sleep();
}

/**
 * Receiving a message in the current data transfer mode. Checking flags and printing data.
 * Returning to ready mode after reception.
 */
void test_spirit_rx_hello() {
	term_log("\e[1mtest_spirit_data_rx_hello()\e[0m");

	spirit_command(COMMAND_FLUSHRXFIFO);

	isr_flags = 0;
	spirit_enter(SPIRIT_STATE_RX);
	term_log("Listening...");

	while (!isr_flag_isset_with_clear(ISR_SPIRIT_IRQ) && p_spirit_irq_val()) {
		__low_power_mode_3();
	}

	p_led_h();
	uint8_t interrupt_source = spirit_check_irq0(0xff);
	term_log_begin();
	term_print("Interrupt_source = RX_DATA_READY? ");
	term_test_result(interrupt_source & IRQ_STATUS0_RX_DATA_READY);
	term_print(" (0b");
	term_binary(interrupt_source, 8);
	term_print(")");
	term_end();

	uint8_t len;
	spirit_read_register_burst(RX_PCKT_LEN0_BASE, 1, &len);
	term_log_begin();
	term_print("len = ");
	term_uint(len, 3);
	term_putchar(' ');
	term_test_result(len == 5);
	term_end();

	uint8_t buffer[32];
	spirit_read_register_burst(0xff, len, buffer);
	buffer[len] = 0;
	term_log_begin();
	term_print("message: \"");
	term_print((char *) buffer);
	term_putchar('\"');
	term_end();

	p_led_l();
	spirit_enter(SPIRIT_STATE_READY);
}

/**
 * Main loop for receiving messages and displaying these over the UART terminal as hex values.
 * Entering LPM3 while receiving.
 */
void test_spirit_rx_loop() {
	term_log("\e[1mtest_spirit_data_rx_loop()\e[0m");
	spirit_rx_start();

	while (1) {
		if (isr_flag_isset_with_clear(ISR_SPIRIT_IRQ) || !p_spirit_irq_val()) {
			if (spirit_rx_is_msg_present()) {
				p_led_h();

				spirit_rx_download_info();
				test_spirit_print_msg();
				test_spirit_print_msg_info();
				term_log(" ");

				p_led_l();
			}
		} else {
			__low_power_mode_3();
		}
	}
}

/**
 * printing the current spirit_msg to the terminal.
 * Printing the message length as a decimal value. Followed by the data as hex values.
 */
void test_spirit_print_msg() {
	term_log_begin();
	term_print("LEN=");
	term_uint(spirit_msg.len, 2);
	term_print(": ");
	for (uint8_t i = 0; i < spirit_msg.len; i++) {
		term_hex(spirit_msg.data[i], 2);
		term_putchar(' ');
	}
	term_end();
}

/**
 * printing the current spirit_msg_info to the terminal.
 */
void test_spirit_print_msg_info() {
	term_log_begin();

	term_print("PQI=");
	term_uint(spirit_msg_info.pqi, 3);
	term_print("  SQI=");
	term_uint(spirit_msg_info.sqi & 0x7f, 3);
	term_print("  RSSI=");
	term_int((spirit_msg_info.rssi >> 1) - 130, 3);
	term_end();

	term_log_begin();
	term_print("CTRL=");
	for (uint8_t i = 0; i < 4; i++) {
		term_hex(spirit_msg_info.ctrl[i], 2);
		term_putchar(' ');
	}

	term_print(" SRC=");
	term_hex(spirit_msg_info.src_addr, 2);
	term_print("  DEST=");
	term_hex(spirit_msg_info.dest_addr, 2);

	term_end();
}

/**
 * Send a wrong message if the button is pressed, otherwise a correct message is send.
 * In case of a wrong message the CRC bit is deactivated.
 */
void test_spirit_wrong_msg_btn() {
	p_led_h();
	term_log("\e[1mtest_spirit_wrong_msg_btn()\e[0m");
    uint8_t crc_mode = 0; //p_btn_val() ? 1 : 0;
	spirit_write_register(PCKTCTRL1_BASE, (crc_mode << 5) + (SPIRIT_PCK_WHIT_EN << 4));
	spirit_tx_data(5, "Hello");
	uint8_t result = spirit_tx_wait_on_finished(1638);

	term_log_begin();
	term_print("result ");
	term_test_result(result);
	term_end();
	p_led_l();
}

/**
 * Send a wrong and a correct message directly one after the other.
 * In case of a wrong message the CRC bit is deactivated.
 */
void test_spirit_double_message() {
	term_log("\e[1mtest_spirit_double_message()\e[0m");
	p_led_h();
	spirit_write_register(PCKTCTRL1_BASE, (0 << 5) + (SPIRIT_PCK_WHIT_EN << 4));
	spirit_tx_data(5, "Hello");
	spirit_tx_wait_on_finished(1638);

	spirit_write_register(PCKTCTRL1_BASE, (1 << 5) + (SPIRIT_PCK_WHIT_EN << 4));
	spirit_tx_data(5, "Hello");
	spirit_tx_wait_on_finished(1638);
	p_led_l();
}

/**
 * Send a test message, wait for an acknowledgment.
 * Print the received data.
 */
void test_spirit_tx_rx_ack() {
	term_log("\e[1mtest_spirit_tx_rx_ack()\e[0m");

	spirit_wakeup();

//	const uint8_t tx_ctrl[] = { 0x12, 0x23, 0x34, 0x45 };
//	spirit_tx_set_ctrl(4, tx_ctrl);
//	const uint8_t rx_ctrl_mask[] = { 0xFF, 0xFF, 0xFF, 0xFF };
//	const uint8_t rx_ctrl[] = { 0xFF, 0xEE, 0xDD, 0xCC };
//	spirit_rx_set_ctrl(4, rx_ctrl_mask, rx_ctrl);

//	spirit_tx_set_addr(0x2E);
//	spirit_tx_data(5, "Hello");
//	spirit_tx_wait_on_finished(1638);

	spirit_tx_set_addr(0x2F);
	spirit_tx_data(5, "Hello");
	spirit_tx_wait_on_finished(1638);

	spirit_rx_set_addr(0x15);
	spirit_rx_start();
	uint8_t len = spirit_rx_timeout(1638);
	spirit_rx_download_info();
	spirit_rx_stop();
	spirit_sleep();

	p_led_h();
	term_log_begin();
	term_print("RX? ");
	term_test_result(len > 0);
	term_end();
	if (len > 0) {
		test_spirit_print_msg();
		test_spirit_print_msg_info();
	}
	term_log(" ");
	p_led_l();
}

/**
 * Receive messages for an infinite time. On received message send an acknowledgment.
 * Print the received data.
 */
void test_spirit_rx_loop_ack() {
	term_log("\e[1mtest_spirit_rx_loop_ack()\e[0m");

    spirit_wakeup();
	spirit_rx_set_addr(0x2F);
	spirit_tx_set_addr(0x15);
//	const uint8_t ctrl[] = { 0xFF, 0xEE, 0xDD, 0xCC };
//	spirit_tx_set_ctrl(4, ctrl);
//	const uint8_t rx_ctrl_mask[] = { 0xFF, 0xFF, 0xFF, 0xFF };
//	const uint8_t rx_ctrl[] = { 0x12, 0x23, 0x34, 0x45 };
//	spirit_rx_set_ctrl(4, rx_ctrl_mask, rx_ctrl);

	spirit_rx_start();

	while (1) {
		if (isr_flag_isset_with_clear(ISR_SPIRIT_IRQ) || !p_spirit_irq_val()) {
			if (spirit_rx_is_msg_present()) {
				spirit_rx_download_info();
				spirit_rx_stop();
				spirit_tx_data(9, "Good day!");
				spirit_tx_wait_on_finished(1638);
				spirit_rx_start();

				p_led_h();
				test_spirit_print_msg();
				test_spirit_print_msg_info();
				term_log(" ");
				p_led_l();
			}
		} else {
			__low_power_mode_3();
		}
	}
}
