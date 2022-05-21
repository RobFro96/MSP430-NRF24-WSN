/**
 * MSP430 and NRF24L01 based WSN
 * @file isr.c
 * @author Robert Fromm
 * @date 06.02.2022
 *
 * Module for handling all common interrupt service routines.
 * Using variable isr_flags and enum isr_flags_t for storing all pending
 * after-return-from-lpm-actions.
 */

#include <msp430.h>
#include <stdint.h>
#include "pins.h"

#include "isr.h"

volatile uint16_t isr_flags = 0;	//! flags of all used interrupts

/**
 * Check if one of the given flags isset. And afterward clear all the given flags.
 *
 * @param flags interrupt flags to be checked and cleared
 * @return zero if no flag was set.
 */
uint16_t isr_flag_isset_with_clear(uint16_t flags) {
	uint16_t result = isr_flags & flags;
	isr_flags &= ~(flags);
	return result;
}

/**
 * Set the ISR_TIMER_DELAY flag after the given number of ACLK cycles.
 * @param aclk_cycles number of ACLK cycles
 */
void isr_timeout(uint16_t aclk_cycles) {
	isr_flags &= ~ISR_TIMER_DELAY;
	TA0CCR0 = aclk_cycles;
	TA0CCTL0 = CCIE;
	TA0CTL = TASSEL_1 + MC_2 + TACLR;
}

/**
 * Stop the timer
 */
void isr_disable_timer() {
	TA0CCTL0 = 0;
	TA0CTL = 0;
}

/**
 * Wait a number of ACLK cycles in Low-Power-Mode
 * @param aclk_cycles number of ACLK cycles
 */
void isr_delay(uint16_t aclk_cycles) {
	isr_timeout(aclk_cycles);

	while (!isr_flag_isset_with_clear(ISR_TIMER_DELAY)) {
		__low_power_mode_3();
	}
}

/**
 * Wait a number of milliseconds, assuming P_CLOCK_ALCK is set correctly
 * @param ms milliseconds
 */
void isr_delay_ms(uint16_t ms) {
    isr_timeout(P_CLOCK_ACLK * ms);

    while (!isr_flag_isset_with_clear(ISR_TIMER_DELAY)) {
        __low_power_mode_3();
    }
}


/**
 * Interrupt Port1
 */
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR() {
}

/**
 * Interrupt Port2
 */
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR() {
    if (p_spirit_irq_ifg()) {
        isr_flags |= ISR_SPIRIT_IRQ;
        p_spirit_irq_ifg_clr();
        __low_power_mode_off_on_exit();
    }
}

/**
 * Interrupt Timer A0 CCR0
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR() {
	isr_flags |= ISR_TIMER_DELAY;
	TA0CCTL0 = 0;
	TA0CTL = 0;
	__low_power_mode_off_on_exit();
}
