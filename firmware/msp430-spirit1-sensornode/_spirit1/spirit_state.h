#ifndef SPIRIT1_SPIRIT_STATE_H_
#define SPIRIT1_SPIRIT_STATE_H_

// Values of the MC_STATE register for the different states
#define SPIRIT_STATE_CODE_STANDBY (0x40 << 1)
#define SPIRIT_STATE_CODE_SLEEP (0x36 << 1)
#define SPIRIT_STATE_CODE_READY (0x03 << 1)
#define SPIRIT_STATE_CODE_LOCK (0x0F << 1)
#define SPIRIT_STATE_CODE_RX (0x33 << 1)
#define SPIRIT_STATE_CODE_TX (0x5F << 1)

typedef struct {
	uint8_t command;
	uint8_t state_code;
} spirit_state_t;

typedef enum {
	SPIRIT_STATE_STANDBY = 0,
	SPIRIT_STATE_SLEEP,
	SPIRIT_STATE_READY,
	SPIRIT_STATE_RX,
	SPIRIT_STATE_TX,
	SPIRIT_STATE_LOCKRX,
	SPIRIT_STATE_LOCKTX
} spirit_states_t;

extern const spirit_state_t spirit_states[];
extern spirit_states_t spirit_current_state;

void spirit_enter_no_wait(spirit_states_t state_id);
void spirit_enter(spirit_states_t state_id);
void spirit_reset();
void spirit_do_calibration();

/**
 * Enter the sleep state.
 */
inline void spirit_sleep() {
	spirit_enter_no_wait (SPIRIT_ULP_STATE);
}

/**
 * Enter the READY state
 */
inline void spirit_wakeup() {
	spirit_enter(SPIRIT_STATE_READY);
}

#endif /* SPIRIT1_SPIRIT_STATE_H_ */
