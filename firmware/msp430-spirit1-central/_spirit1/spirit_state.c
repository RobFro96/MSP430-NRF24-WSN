/**
 * MSP430 and NRF24L01 based WSN
 * @file spirit_state.c
 * @author Robert Fromm
 * @date 06.02.2022
 *
 * Functions to control the states of the SPIRIT1
 */

#include <msp430.h>
#include <stdint.h>

#include "spirit.h"
#include "spirit_state.h"

const spirit_state_t spirit_states[] = {
		{ COMMAND_STANDBY, SPIRIT_STATE_CODE_STANDBY },
		{ COMMAND_SLEEP, SPIRIT_STATE_CODE_SLEEP },
		{ COMMAND_READY, SPIRIT_STATE_CODE_READY },
		{ COMMAND_RX, SPIRIT_STATE_CODE_RX },
		{ COMMAND_TX, SPIRIT_STATE_CODE_TX },
		{ COMMAND_LOCKRX, SPIRIT_STATE_CODE_LOCK },
		{ COMMAND_LOCKTX, SPIRIT_STATE_CODE_LOCK }
};

spirit_states_t spirit_current_state = SPIRIT_STATE_STANDBY; //! current state of the SPIRIT1 module

static inline uint8_t is_in_state(uint16_t status, uint8_t state) {
	return (status & 0x00fe) == state;
}

/**
 * Enter a state. Just sending the command. No waiting.
 *
 * @param state_id state to enter
 */
void spirit_enter_no_wait(spirit_states_t state_id) {
	if (spirit_current_state == state_id) {
		return;
	}

	const spirit_state_t *state = &spirit_states[state_id];
	spirit_command(state->command);
	spirit_current_state = state_id;
}

/**
 * Enter a state. Wait until the state is entered.
 * If the config macro SPIRIT_ENTER_WDT_ENABLE isset and SPIRIT gets stucked,
 * the watchdog timer perform a reset.
 *
 * @param state_id state to enter
 */
void spirit_enter(spirit_states_t state_id) {
	if (spirit_current_state == state_id) {
		return;
	}

#if SPIRIT_ENTER_WDT_ENABLE
	WDTCTL = WDTPW + WDTCNTCL + SPIRIT_ENTER_WDT_INTERVAL;
#endif

	const spirit_state_t *state = &spirit_states[state_id];

	uint16_t status = spirit_command(state->command);
	while (!is_in_state(status, state->state_code)) {
		status = spirit_get_status();
	}

#if SPIRIT_ENTER_WDT_ENABLE
	WDTCTL = WDTPW | WDTHOLD;
#endif

	spirit_current_state = state_id;
}

/**
 * Resetting the module.
 */
void spirit_reset() {
	// Reset
	spirit_command(COMMAND_SRES);
	spirit_current_state = SPIRIT_STATE_STANDBY;
	spirit_enter(SPIRIT_STATE_READY);
}

/**
 * Manual calibration of the VCO. Both TX and RX calibration values are generated.
 * The runtime of the whole function is about 400 µs @ 8 MHz MCLK or 1.5 ms @ 1 MHz
 */
void spirit_do_calibration() {
	uint8_t cal_reg;
	spirit_write_register(PROTOCOL2_BASE, 0b00000010); // enable vco calibration

	spirit_enter(SPIRIT_STATE_LOCKTX);
	spirit_read_register_burst(RCO_VCO_CALIBR_OUT0_BASE, 1, &cal_reg);
	spirit_write_register(RCO_VCO_CALIBR_IN1_BASE, cal_reg);

	spirit_enter(SPIRIT_STATE_READY);

	spirit_enter(SPIRIT_STATE_LOCKRX);
	spirit_read_register_burst(RCO_VCO_CALIBR_OUT0_BASE, 1, &cal_reg);
	spirit_write_register(RCO_VCO_CALIBR_IN0_BASE, cal_reg);

	spirit_enter(SPIRIT_STATE_READY);

#if !SPIRIT_AUTO_VCO
	spirit_write_register(PROTOCOL2_BASE, 0b00000000); // disable vco calibration
#endif
}
